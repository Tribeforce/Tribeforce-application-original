<?php
// Copyright (c) 2013 Femi Veys


/***************
 Page callbacks
***************/
function person_details($nid) {
  $regions = array();
  $node = node_load($nid);
  $enriched_person = get_enriched_person(array('node' => $node));

  tf_help_msg(t('Add coaching remarks, review skills and talents or edit
                         employee details by clicking on his or her picture.'));

  $empty_regions = array(
    'left-sidebar' => array(),
    'content' => tf_manage_person_details_ra($enriched_person),
  );

  return tf_module('manage', $empty_regions, $regions);
}



/***************
 Render Arrays
***************/
function tf_manage_person_details_ra($enriched_person) {
  $tabs = array(
    'remarks' => array('#markup' => t('Remarks')),
    'skills' => array('#markup' => t('Skills')),
    'talents' => array('#markup' => t('Talents')),
  );

  module_load_include('inc', 'tf_manage', 'overview');
  $remarks_pane = container_ra(array(
    'new' => add_remark_ra($enriched_person),
    'list' => empty($enriched_person['remarks']) ? array() :
           items_ra($enriched_person['remarks'], 'remark',
                               $enriched_person['node']->nid, 'person-details'),
  ));

  $panes = array(
    'remarks' => $remarks_pane,
    'skills' => empty($enriched_person['skills']) ? array() :
         items_ra($enriched_person['skills'], 'skill', 0, 'person-details'),
    'talents' => empty($enriched_person['talents']) ? array() :
       items_ra($enriched_person['talents'], 'talent', 0, 'person-details'),
  );

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('person-details'),
    ),
    'data' =>   container_ra(array(
      'col-1' => item_ra($enriched_person, 'person-details'),
      'col-2' => tabs_widget_ra($tabs, $panes),
    )),
  );

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
