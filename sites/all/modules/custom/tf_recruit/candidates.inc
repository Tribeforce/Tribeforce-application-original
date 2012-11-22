<?php

/*
 Forms
*/
/*
function candidate_form($form, &$form_state, $node) {
  // Get the form elements of the node
  field_attach_form('node', $node, $form, $form_state);

  $fields = array(
    'field_first_name' => $form['field_first_name'],
    'last_name' => array(
      '#title' => t('Last name'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#default_value' => empty($node->title) ? '' : $node->title,
      '#weight' => 2,
    ),
    'field_email' => $form['field_email'],
    'field_phone' => $form['field_phone'],
    'field_cv' => $form['field_cv'],
    'field_motivation' => $form['field_motivation'],
  );

  $buttons = container_ra(array(
    'cv' => container_ra(array('button' => array())),
    'motivation' => container_ra(array('button' => array())),
    'actions' => array(
//      '#weight' => 500,
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Add'),
      ),
    ),
  ));

  $img = theme_image(
    array(
      'path' => '/sites/all/themes/tf_app/images/person.png',
      'attributes' => array(),
//        'alt' => 'person',
//        'title' => 'person',
//      ),
//      'image_style' => 'thumbnail',
//      'path' => ,
    )
  );

  return container_ra(array(
    'icon' => array(
      '#markup' => $img,
    ),
    'fields' => $fields,
    'buttons' => $buttons,
  ));
}
*/

function compare_form($form, &$form_state, $campaign_nid) {
  return array(
    'selected' => array(
      '#type' => 'hidden',
    ),
    'compare' => array(
      '#type' => 'submit',
      '#value' => t('Compare Candidates'),
      '#ajax' => array(
        'callback' => 'compare_candidates_callback',
//        'progress' => array('type' => 'none'),
      ),
    ),
    'hire' => array(
      '#type' => 'submit',
      '#value' => t('Hire'),
      '#submit' => array('hire_submit'),
    ),
  );
}


function interview_interpretation_form($form, &$form_state, $node) {
  // Get the form elements of the node
  field_attach_form('node', $node, $form, $form_state);

  return array(
    'score' => $form['field_score'],
//    'done' => $form['field_done'],
    'notes' => array(
      '#type' => 'textarea',
      '#default_value' => $node->title,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Done'),
      '#ajax' => array(
        'callback' => 'interview_interpretation_callback',
        'progress' => array('type' => 'none'),
      ),
    ),
  );
}



/*
 Forms submits
*/
/*
function candidate_form_submit($form, &$form_state) {
  $person = $form_state['build_info']['args'][0];
  $person->title = $form_state['values']['last_name'];
  $person->field_first_name = $form_state['values']['field_first_name'];
  $person->field_email = $form_state['values']['field_email'];
  $person->field_phone = $form_state['values']['field_phone'];
  $person->field_person_type = array('und' => array(array('value' => 1)));

  // Check if a file has been uploaded
  if($form_state['values']['field_cv']['und'][0]['fid'] != 0)
    $person->field_cv = $form_state['values']['field_cv'];

  // Check if a file has been uploaded
  if($form_state['values']['field_motivation']['und'][0]['fid'] != 0)
    $person->field_motivation = $form_state['values']['field_motivation'];

  node_save($person);
//  $form_state['redirect'] = dirname(current_path());
}
*/


function hire_submit($form, &$form_state) {
  $candidate_nids_string = $form_state['values']['selected'];
  $campaign_nid = $form_state['build_info']['args'][0];
  // It is assumed $candidate_nids holds just 1 value
  $person_nid = $candidate_nids_string;

  drupal_goto("recruit/$campaign_nid/candidates/hire/$person_nid");
}



/*
Page callbacks
*/
function candidates($campaign_nid, $candidate_nid = 0) {
  $persons = get_candidates($campaign_nid);
  // Returns FALSE if no node given or nid doesn not exist
  $candidate = node_load($candidate_nid);

  $message = array(
    '#markup' => t('Here you can add candidates.'),
  );

  return recruit(array(
    'message' => $message,
    'content' => candidates_ra($campaign_nid, $persons, $candidate),
  ));

}

function interview($interview_nid) {
  $interview = get_interview($interview_nid);
  $person_nid = $interview->field_person_back_ref['und'][0]['nid'];
  $person = get_person($person_nid);

  // TODO: This can be in 1 function (see compare)
  $ra = slide_pane_ra(interview_ra($person, $interview));
  $html = drupal_render($ra);

  $commands[] = ajax_command_append('.tf-module', $html);
  $commands[] = ajax_command_invoke('.tf-region.slide-pane',
                                                   'addClass', array('open'));
  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
}

function add_interview($campaign_nid, $person_nid) {
  // Create interview node
  $interview = new stdClass();
  $interview->type = 'interview';
  node_object_prepare($interview);

  // TODO: What title should it be?
  $interview->title = "Interview for $person_nid";

  // Link the interview to the person and the campaign
  // TODO: populate in a proper way field_view_field
  $interview->field_person_back_ref['und'][] = array('nid' => $person_nid);
  $interview->field_campaign_ref['und'][] = array('nid' => $campaign_nid);

  node_save($interview);

  $role_nid =  get_role_nid_of_campaign($campaign_nid);
  prepare_interpretations(array($role_nid), $interview);

  interview($interview->nid);
}

function add_candidate($campaign_nid, $candidate) {
  if(empty($candidate)) {
    $candidate = new stdClass();
    $candidate->type = 'person';
    node_object_prepare($candidate);

    // Set some default fields
    $candidate->field_company_ref = tf_em_get_company();
    $candidate->field_person_type = array('und' => array(array('value' => 1)));
    $candidate->field_campaigns_ref = array(
      'und' => array(array('nid' => $campaign_nid))
    );

    // TODO: Find source of bug http://drupal.org/node/1824820
    //       It has something to do with the avatar that doesn't exist for a new person
    $candidate->field_avatar = get_default_avatar();
  }

  $extra = array(
    'original_nid' => $campaign_nid,
  );

  module_load_include('inc', 'tf_manage', 'person_edit');
  return tf_manage_person_edit_ra($candidate, 'candidate', $extra);
}




/*
AJAX callbacks
*/
function interview_interpretation_callback($form, $form_state) {
  // Get the node from the build_info variables in the form_state
  $node = $form_state['build_info']['args'][0];

  // Update the node fields with the values from the form
  $node->title = $form_state['values']['notes'];
  $node->field_score = $form_state['values']['score'];
  $node->field_done['und'][0]['value'] = 1;

  node_save($node);
}



function compare_candidates_callback($form, $form_state) {
  $candidate_nids_string = $form_state['values']['selected'];
  $campaign_nid = $form_state['build_info']['args'][0];

  module_load_include('inc', 'tf_recruit', 'compare');

  // TODO: This can be in 1 function (See interview)
  $ra = slide_pane_ra(compare($campaign_nid,$candidate_nids_string));
  $html = drupal_render($ra);
  $commands[] = ajax_command_append('.tf-module.recruit', $html);
  $commands[] = ajax_command_invoke('.tf-region.slide-pane',
                                                   'addClass', array('open'));
  return array('#type' => 'ajax', '#commands' => $commands);
}


/*
Render Arrays
*/
// TODO: 1 function?
function candidate_ra($campaign_nid, $person, $index = 0) {
  // Link in the interview
  // TODO: optimize
  $interview_ref = $person->field_assessments_ref;
  if(!empty($interview_ref)) {
    foreach($interview_ref['und'] as $interview_ref_el) {
      $interview_temp = node_load($interview_ref_el['nid']);
      if(!empty($interview_temp) && $interview_temp
                     ->field_campaign_ref['und'][0]['nid'] == $campaign_nid) {
        $interview = $interview_temp;
        break;
      }
    }
  }

  if(empty($interview)) {
    $interview_ra = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('add-interview'),
      ),
      '#theme' => 'link',
      '#text' => '',
      '#weight' => 60,
      '#path' => "recruit/$campaign_nid/nojs/candidates/add_interview/" . $person->nid,
      '#options' => array(
        'attributes' => array('class' => array('use-ajax')),
        'html' => FALSE,
      ),
    );
  } else {
    $interview_ra = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('interview-' . $interview->nid),
      ),
      '#theme' => 'link',
      '#text' => '',
      '#weight' => 60,
      '#path' => "recruit/$campaign_nid/nojs/candidates/interview/" . $interview->nid,
      '#options' => array(
        'attributes' => array('class' => array('use-ajax')),
        'html' => FALSE,
      ),
    );
  }

  $role = node_load(get_role_nid_of_campaign($campaign_nid));
  $categorized_scores = get_categorized_scores($role, array($person));
  if(empty($categorized_scores['candidate-'.$person->nid])) {
    $score = container_ra(array('global-score' => array('#markup' => 0)));
  } else {
    $score = candidate_score_ra($categorized_scores['candidate-'.$person->nid], 'global');
  }

  $data =   array(
    'first_name' => field_view_field('node', $person, 'field_first_name',
                                   array('label' => 'hidden', 'weight' => 0)),
    'last_name' => field_view_title($person, 10),
    'email' => field_view_field('node', $person, 'field_email',
                                  array('label' => 'hidden', 'weight' => 20)),
    'phone' => field_view_field('node', $person, 'field_phone',
                                  array('label' => 'hidden', 'weight' => 30)),
    'edit-link' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('edit-link')),
      '#theme' => 'link',
      '#text' => t('edit'),
      '#path' => "recruit/$campaign_nid/candidates/" . $person->nid . "/edit",
      '#options' => array(
        'attributes' => array(/*'class' => array('use-ajax')*/),
        'html' => FALSE,
      ),
    )
  );

  $buttons = container_ra(array(
    'interview' => $interview_ra,
    'cv' => field_view_field('node', $person, 'field_cv',
                                  array('label' => 'hidden')),
    'motivation' => field_view_field('node', $person, 'field_motivation',
                                  array('label' => 'hidden')),
  ));

  $graph = array();

  $even_odd = $index % 2 == 0 ? 'even' : 'odd';

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'candidate',
        'candidate-nid-' . $person->nid,
        'candidate-' . $index,
        'candidate-' . $even_odd,
        'clearfix',
      ),
    ),
    'data' => container_ra(array(
      'record' => container_ra(array(
        'left' => container_ra(array(
          'score' => $score,
          'selected' => array('#markup' => t('Select Candidate')),
          'not-selected' => array('#markup' => t('Selected')),
        )),
        'data' => $data,
        'buttons' =>  $buttons,
      )),
//      'graph' => $graph,
    )),
  );
}

function candidate_score_ra($score, $type) {
  if(isset($score[$type])) {
    return array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array("$type-score"),
      ),
      'score' => array(
        '#markup' => round($score[$type], 1),
        '#prefix' => '<div class="field-label">'
                   .    t(ucfirst($type). ' score').': '
                   . '</div>',
      ),
    );
  }
  else {
    // TODO: What to do if a certain score type has not been set (now we hide)
  }
}


function candidates_ra($campaign_nid, $persons, $candidate = FALSE) {
  $index = 0;
  $candidates = array();

  foreach($persons as $person) {
    $candidates['person-'. $person->nid] =
      candidate_ra($campaign_nid, $person, $index);
    $index++;
  }

  $selection_pane = container_ra(array(
    'selection-0' => t('Please select a candidate you would like to hire.'),
    'selection-1' => t('Hire this candidate or select another to compare.'),
    'selection-2' => t('Compare these candidates or select a last one to compare.'),
    'selection-3' => t('Compare these candidates.'),
    'compare' => drupal_get_form('compare_form', $campaign_nid),
  ));


  $header = container_ra(array(
    'add' => add_candidate($campaign_nid, $candidate),
    'selection-pane' => $selection_pane,
  ));


  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('candidates-page'),
    ),
    'data' =>   container_ra(array(
      'header' => $header,
      'candidates' => $candidates,
   )),
  );
}

function interview_ra($person, $interview) {
  return array(
    'page' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('interview', 'interview-' . $interview->nid),
      ),
      'header' => interview_header_ra($person, $interview),
      'print' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('print'),
        ),
        'data' => array(
          '#markup' => t('print'),
        ),
      ),
      'body' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('body'),
        ),
        'data' => interview_questions_ra($interview),
      ),
    ),
  );
}

function interview_header_ra($person, $interview) {
  switch($interview->type) {
    case 'interview':
      $title = field_view_title(get_campaign($interview), 0);
      break;
    case 'evaluation':
      // TODO: This is some kind of hack
      $title = field_view_title($interview, 0);
      break;
  }
//  $campaign = get_campaign($interview);

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('header'),
    ),
    'title' => $title,
    'date' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('field', 'date'),
      ),
      '#weight' => 10,
      'data' => array(
        '#markup' => format_date(time(), 'short_date'),
      ),
    ),
    'first_name' => field_view_field('node', $person,
              'field_first_name', array('label' => 'hidden', 'weight' => 20)),
    'last_name' => field_view_title($person, 30),
    'cv' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'field',
          'field-name-field-cv',
          'field-type-file'
        ),
      ),
      '#weight' => 40,
      '#theme' => 'link',
      '#text' => t('CV'),
      '#path' => file_create_url($person->field_cv['und'][0]['uri']),
      '#options' => array(
        'attributes' => array(),
        'html' => FALSE,
      ),
    ),
    'email' => field_view_field('node', $person,
                   'field_email', array('label' => 'hidden', 'weight' => 50)),
    'phone' => field_view_field('node', $person,
                   'field_phone', array('label' => 'hidden', 'weight' => 60)),
    'motivation' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'field',
          'field-name-field-motivation',
          'field-type-file'
        ),
      ),
      '#weight' => 70,
      '#theme' => 'link',
      '#text' => t('Motivation letter'),
      '#path' => file_create_url($person->field_motivation['und'][0]['uri']),
      '#options' => array(
        'attributes' => array(),
        'html' => FALSE,
      ),
    ),

  );
}



function interview_questions_ra($interview) {
  $index = 0;
  foreach($interview->field_interprets_ref['und']
                 as $key => $interpretation_index) {

    // TODO: Optimize these 2 queries
    $interpretation = node_load($interpretation_index['nid']);
    $interview_item = node_load($interpretation
                                   ->field_ass_item_ref['und'][0]['nid']);

    $even_odd = $index % 2 == 0 ? 'even' : 'odd';

    if(!empty($interpretation->field_done)) {
      $done = ($interpretation->field_done['und'][0]['value'] == 1);
    }
    else {
      $done = FALSE;
    }

    $done_ra = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('done')),
      'text' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('text')),
        'data' => array(
          '#markup' => t('done'),
        ),
      ),
      'score' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('score')),
        'data' => array(
          '#markup' => $interpretation->field_score['und'][0]['value'],
        ),
      ),
    );

    $ra["question-$index"] = array( //TODO: choose a proper key name
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'question-wrapper',
          'question-wrapper-' . $index,
          'question-wrapper-' . $even_odd,
          'collapsed',
          $done ? 'done' : '',
        ),
      ),
      'header' => array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('header'),
        ),
        'title' => field_view_title($interview_item, 0),
        'done' => $done_ra,
        'description' => field_view_field('node', $interview_item,
              'field_question', array('label' => 'hidden', 'weight' => 10)),
      ),

      'data' => interview_question_ra($interview_item,
                                      $interpretation, $interview->type),
    );

    if($interview->type === 'evaluation') {
      unset($ra["question-$index"]['header']['description']);
    }

    $index++;
  }

  return $ra;
}

function interview_score_ra($score, $selected) {
  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('score', 'score-' . $score, $selected),
    ),
    'data' => array(
      '#markup' => $score > 0 ? '+' . $score : $score,
    ),
  );
}


function interview_scores_ra($score) {
  $ra = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('scores'),
    ),
  );

  for($i = 2; $i >= -2; $i--) {
    $selected = $score == $i ? 'selected' : '';
    $ra['score-' . $i] = interview_score_ra($i, $selected);
  }

  return $ra;
}



function interview_question_ra($interview_item, $interpretation = NULL, $interview_type){
  $type = str_replace('_', '-', $interview_item->type);
  $nid = $interview_item->nid;
  $score = $interpretation->field_score['und'][0]['value'];

  $col1 = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array("col", "col-1"),
    ),
  );
  switch($interview_type) {
    case 'interview':
      $col1['pos_hint'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('pos-hint'),
        ),
        'data' => field_view_field('node', $interview_item,'field_pos_answer'),
      );
      $col1['neg_hint'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('neg-hint'),
        ),
        'data' => field_view_field('node', $interview_item,'field_neg_answer'),
      );
      break;
    case 'evaluation':
      $col1['pos_hint'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('pos-hint'),
        ),
        'data' => field_view_field('node', $interview_item,'field_pos_hint'),
      );
      $col1['neg_hint'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('neg-hint'),
        ),
        'data' => field_view_field('node', $interview_item,'field_neg_hint'),
      );
      break;
  }

  $col2 = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array("col", "col-2"),
    ),
    'data' => interview_scores_ra($score),
  );

  if(empty($interpretation)) {
    $col3 = array();
  }
  else {
    $col3 = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array("col", "col-3"),
      ),
      'data' => drupal_get_form('interview_interpretation_form',
                                                             $interpretation),
    );
  }

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'answer',
        $type . '-answer',
        $type . '-' . $nid . '-answer',
      ),

    ),
    'callapse-container' => container_ra(array(
      'more' => array('#markup' => t('more')),
      'answer-body' => array(
        'col1' => $col1,
        'col2' => $col2,
        'col3' => $col3,
      ),
      'less' => array(),
    )),
  );
}


/*
Helpers
*/

// TODO: do real calculation
function get_categorized_scores($role, $candidates) {
  $categorized_scores = array();

  // Fill the array with all values in the right order to be able to take
  // an average
  foreach(get_scores($role, $candidates) as $type_key => $type) {
    dpm($type_key);
    foreach($type as $item_key => $item) {
      foreach($item as $can_key => $candidate) {
        switch($type_key) {
          case 'skill':
            $cs_type_key = 'skill';
            break;
          case 'talent_attribute':
            $cs_type_key = 'ta';
            break;
/*
          case 'behavioral_ta':
            $cs_type_key = 'behavioral';
            break;
*/
        }
        $categorized_scores[$can_key][$cs_type_key][] = $candidate['score'];
      }
    }
  }

  // Calculate the averages
  foreach($categorized_scores as &$candidate_scores) {
    foreach($candidate_scores as &$type_scores) {
      $type_scores = array_sum($type_scores) / count($type_scores);
    }
    $candidate_scores['global'] =
      array_sum($candidate_scores) / count($candidate_scores);
  }

  return $categorized_scores;
}
