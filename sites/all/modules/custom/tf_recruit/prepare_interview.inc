<?php

/*
 Forms
*/
function prepare_interview_form($form, &$form_state, $node, $locked = FALSE) {
  $lang = LANGUAGE_NONE; //TODO

  $form = array();

  if($node->type === 'skill' or $node->type === 'talent_attribute') {
    $form['assess'] = array(
      '#type' => 'hidden',
      '#default_value' => empty($node->field_assess) ?
                                  '' : $node->field_assess[$lang][0]['value'],
    );
    $form['question'] = array(
      '#type' => 'textarea',
      '#default_value' => empty($node->field_question) ?
                                '' : $node->field_question[$lang][0]['value'],
      '#attributes' => array('placeholder' => t('Put your question here.')),
    );
    $form['pos_answer'] = array(
      '#title' => t('Positive answer'),
      '#type' => 'textarea',
      '#default_value' => empty($node->field_pos_answer) ?
                                '' : $node->field_pos_answer[$lang][0]['value'],
    );
    $form['neg_answer'] = array(
      '#title' => t('Negative answer'),
      '#type' => 'textarea',
      '#default_value' => empty($node->field_neg_answer) ?
                                '' : $node->field_neg_answer[$lang][0]['value'],
    );
  }

  $form = array_merge($form, array(
    'actions' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('actions')),
      'confirm' => array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#attributes' => array('class' => array('btn', 'btn-primary')),
        '#ajax' => array(
          'callback' => 'prepare_interview_callback',
          'progress' => array('type' => 'none'),
        ),
      ),
    ),
  ));

  return $form;
}



/*
Page callbacks
*/
function prepare_interview($campaign_nid) {
  $role_nid = get_role_nid_of_campaign($campaign_nid);
  $full_role = get_full_role($role_nid);
  $locked = is_campaign_locked($campaign_nid);

  tf_help_msg('Here you can prepare the interview.');

  return recruit(array(
    'menu' => recruit_tabs_ra($campaign_nid),
    'content' => prepare_interview_ra($full_role, $locked),
  ));
}



/*
AJAX callbacks
*/
function prepare_interview_callback($form, $form_state) {
  $lang = LANGUAGE_NONE; //TODO

  // Get the node from the build_info variables in the form_state
  $node = $form_state['build_info']['args'][0];
  $locked = $form_state['build_info']['args'][1];

  // If some hacker enables the fields, we need to make sure nothing can be
  // processed
  if(!$locked) {
    // Update the node fields with the values from the form
    if($node->type === 'skill' or $node->type === 'talent_attribute') {
      $node->field_assess[$lang][0]['value']  = $form_state['values']['assess'];
      $node->field_question[$lang][0]['value']
                                            = $form_state['values']['question'];

  //  if($node->type == 'skill') {
      $node->field_pos_answer[$lang][0]['value']
                                          = $form_state['values']['pos_answer'];
      $node->field_neg_answer[$lang][0]['value']
                                          = $form_state['values']['neg_answer'];
  //  }
    }

    node_save($node);
  }

}



/*
Render Arrays
*/
function prepare_interview_ra($role, $locked = FALSE) {
  $skills = array();
  $business = array();
  $behavioral = array();

  $role_ra = array(
    'name' => field_view_title($role['node'], -10),
    'description' => field_view_field('node', $role['node'],
                              'body', array('label' => 'hidden')),
    'edit-link' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('edit')),
      '#theme' => 'link',
      '#text' => t('Go to role details'),
      '#path' => "roles/" . $role['node']->nid,
      '#options' => array(
        'attributes' => array(),
        'html' => FALSE,
      ),
    ),
  );



  // Skills
  if(!empty($role['skills'])) {
    foreach($role['skills']  as $item) {
      $skills['skill-' . $item['node']->nid] =
        prepare_interview_ra_item($item['node'], $locked);
    }
  }

  // Behavioral Talents
  if(!empty($role['talents']['behavioral'])) {
    foreach($role['talents']['behavioral']  as $item) {
      $behavioral['behavioral-' . $item['node']->nid] =
        prepare_interview_ra_item($item['node'], $locked);
    }
  }

  // Business Talents
  if(!empty($role['talents']['business'])) {
    foreach($role['talents']['business']  as $item) {
      $business['business-' . $item['node']->nid] =
        prepare_interview_ra_item($item['node'], $locked);
    }
  }

  $tabs = array(
   'skills' => array('#markup' => t('Skills')),
   'business' => array('#markup' => t('Business talents')),
   'behavioral' => array('#markup' => t('Behavioral talents')),
  );

  $panes = array(
    'skills' => $skills,
    'business' => $business,
    'behavioral' => $behavioral,
  );

  $tab_pane = tabs_widget_ra($tabs, $panes);

  $ra = array_merge(container_ra(array('role' => $role_ra)), $tab_pane);

  // Return the RA consisting of Skills RA and the Talents RA
  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'prepare-interview',
        $locked ? 'locked' : '',
      ),
    ),
    'data' => $ra,
  );
}

function prepare_interview_ra_item($node, $locked = FALSE) {
  $lang = LANGUAGE_NONE; //TODO
  $tas = array();

  $title_ra['title'] = field_view_title($node);

  // Skill specifics
  if($node->type == 'skill') {
    $title_ra['level'] = flames_ra($node, 'field_expert_level');
    $status = empty($node->field_assess) ? FALSE :
      ($node->field_assess[$lang][0]['value'] == 1 ? TRUE : FALSE);
  }

  $title_ra['switch'] = container_ra(array('switch' => array()));

  // Talent specifics
  if($node->type == 'talent') {
    $tas = tas_ra($node);
    $status = has_tas_to_assess($node);
  }

  $body_ra = container_ra(array(
    'description' => field_view_field('node', $node,
                              'body', array('label' => 'hidden')),
    'form' => drupal_get_form('prepare_interview_form', $node, $locked),
    'tas' => $tas,
  ));


  $nid = $node->nid;
  $type = str_replace('_', '-', $node->type);

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        $type,
        $type . '-' . $nid,
        $status ? 'on' : '',
      ),
    ),
    'data' => container_ra(array(
      'header' => $title_ra,
      'body' => $body_ra,
    )),
  );
}

// Render Talent Attributes
function tas_ra($node) {
  $ra = array();
  $tas = get_tas($node);
  $index = 0;
  foreach($tas as $ta) {
    $ra[] = ta_ra($ta['node'], $index);
    $index++;
  }
  return $ra;
}

// Render a single Talent Attribute
function ta_ra($node, $index = 0) {
  $even_odd = $index % 2 == 0 ? 'even' : 'odd';

  $title_ra['title'] = field_view_title($node);
  $title_ra['switch'] = container_ra(array('switch' => array()));

  $body_ra = container_ra(array(
    'description' => field_view_field('node', $node,
                              'body', array('label' => 'hidden')),
    'form' => drupal_get_form('prepare_interview_form', $node),
//    'tas' => $tas,
  ));

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'ta',
        'ta-' . $index,
        'ta-' . $even_odd,
        'nid-' . $node->nid,
        is_to_assess($node) ? 'on' : '',
      ),
    ),
    'data' => container_ra(array(
      'header' => $title_ra,
      'body' => $body_ra,
    )),


////    'name' => field_view_title($node, -10),
//    'switch' => container_ra(array('switch' => array())),
//    'description' => field_view_field('node', $node,
//                              'body', array('label' => 'hidden')),
  );
}

/*********
 Helpers
 ********/

// This function return TRUE or FALSE
// TRUE if the talent has talent attributes to assess
// FALSE if the talent has not any talent attributes to assess
function has_tas_to_assess($talent) {
  $tas = get_tas($talent);
  $ret = FALSE;
  foreach($tas as $ta) {
    if(is_to_assess($ta['node'])) {
      $ret = TRUE;
      break;
    }
  }
  return $ret;
}

function is_to_assess($ta) {
  return empty($ta->field_assess) ? FALSE :
    ($ta->field_assess['und'][0]['value'] == 1 ? TRUE : FALSE);
}

function is_campaign_locked($campaign_nid) {
  $campaign = node_load($campaign_nid);
  $count = 0;

  if(!empty($campaign->field_interviews_ref['und'])) {
    $count = count($campaign->field_interviews_ref['und']);
  }

  return $count > 0 ? TRUE : FALSE;
}
