<?php
/***************
 Page callbacks
***************/
function person_details($nid) {
  $regions = array();
  $node = node_load($nid);
  $enriched_person = get_enriched_person(array('node' => $node));

  $empty_regions = array(
    'left-sidebar' => array(),
    'message' => array('#markup' => t('Some help text')),
    'content' => tf_manage_person_details_ra($enriched_person),
  );

  return tf_module('manage', $empty_regions, $regions);
}



/***************
 Render Arrays
***************/
function tf_manage_person_details_ra($enriched_person) {
  $tabs = container_ra(array(
    'remarks' => array('#markup' => t('Remarks')),
    'skills' => array('#markup' => t('Skills')),
    'talents' => array('#markup' => t('Talents')),
  ));

  module_load_include('inc', 'tf_manage', 'overview');
  $remarks_pane = container_ra(array(
    'new' => add_remark_ra($enriched_person),
    'list' => empty($enriched_person['remarks']) ? array() :
           items_ra($enriched_person['remarks'], 'remark',
                               $enriched_person['node']->nid, 'person-details'),
  ));


  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('person-details'),
    ),
    'data' =>   container_ra(array(
      'col-1' => item_ra($enriched_person, 'person-details'),
      'col-2' => container_ra(array(
        'tab-pane' => container_ra(array(
          'tabs' => $tabs,
          'panes' => container_ra(array(
            'remarks' => $remarks_pane,
            'skills' => empty($enriched_person['skills']) ? array() :
                 items_ra($enriched_person['skills'], 'skill', 0, 'person-details'),
            'talents' => empty($enriched_person['talents']) ? array() :
               items_ra($enriched_person['talents'], 'talent', 0, 'person-details'),
          )),
        )),
      )),
    )),
  );

}

function tf_manage_evaluation_ra($evaluation, $index) {
   $even_odd = $index % 2 == 0 ? 'even' : 'odd';

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'evaluation',
        'nid-' . $evaluation->nid,
        'index-' . $index,
        $even_odd,
      ),
    ),
    'data' => container_ra(array(
      'date' => array(
        '#markup' => format_date($evaluation->created,'short_date'),
      ),
      'link' => array(
        '#theme' => 'link',
        '#text' => t('Evaluation') . ' ' . ($index+1),
        '#path' => 'recruit/candidates/interview/' . $evaluation->nid,
        '#options' => array(
          'attributes' => array('class' => array('use-ajax')),
          'html' => FALSE,
        ),
      ),
    )),
 );
}

function tf_manage_evaluations_ra($enriched_person) {
  $ra = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('evaluations'),
    ),
  );

  $index = 0;
  foreach($enriched_person['evaluations'] as $eval) {
    $ra['nid-'. $eval->nid] = tf_manage_evaluation_ra($eval, $index);
    $index++;
  }

  $ra['add'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('add'),
    ),
    '#theme' => 'link',
    '#text' => t('Add a new evaluation'),
    '#path' => 'manage/evaluate/' . $enriched_person['node']->nid,
    '#options' => array(
      'attributes' => array(),
      'html' => FALSE,
    ),
  );

  return show_hide(t('Evaluations'), $ra);
}






/*
 Helpers
*/
function add_evaluation($person_nid) {
  // Create evaluation node
  $evaluation = new stdClass();
  $evaluation->type = 'evaluation';
  node_object_prepare($evaluation);

  // TODO: What title should it be?
  $evaluation->title = "Evaluation for $person_nid";

  // Link the interview to the person
  // TODO: populate in a proper way field_view_field
  $evaluation->field_person_back_ref['und'][] = array('nid' => $person_nid);

  node_save($evaluation);

  // TODO: This is probably a bit overkill
  $node = node_load($person_nid);
  $enriched_person = get_enriched_person(array('node' => $node));
  $role_nids = flatten_objects($enriched_person['roles'], 'nid');

  prepare_interpretations($role_nids, $evaluation);

  $next = "manage/person/$person_nid";
  drupal_goto($next);
}
