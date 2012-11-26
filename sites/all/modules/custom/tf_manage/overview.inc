<?php

/******
 Forms
******/
function tf_manage_remark_form($form, &$form_state, $node) {
  return array(
    'remark' => array(
      '#title' => t('Remark'),
      '#type' => 'textarea',
    ),
    'confirm' => array(
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#ajax' => array(
        'callback' => 'add_remark_callback',
        'progress' => array('type' => 'none'),
      ),
    ),
  );
}


/***************
 Page callbacks
***************/
function manage($regions = array()) {
  $my_people = get_enriched_persons();

  $empty_regions = array(
    'left-sidebar' => array(),
    'message' => array('#markup' => t('Click on a tile to add a remark')),
    'content' => manage_overview_ra($my_people),
  );

  return tf_module('manage', $empty_regions, $regions);
}


/***************
 AJAX callbacks
***************/
function add_remark_callback($form, $form_state) {
  // Get the node from the build_info variables in the form_state
  $person = $form_state['build_info']['args'][0];

  $remark_txt = trim($form_state['values']['remark']);

  if(!empty($remark_txt)) {
    // Create the Remark node
    $remark = new stdClass();
    $remark->type = 'remark';
    node_object_prepare($remark);

    // Populate the fields
    $remark->title = $remark_txt;
    $remark->field_person['und'][0]['nid'] = $person->nid;

    // Save the Remark node
    node_save($remark);

    // Hide the popups
    $commands[] = ajax_command_invoke(NULL, "invoke_hide_remarks");
    return array('#type' => 'ajax', '#commands' => $commands);
  }
}


/***************
 Render Arrays
***************/
function manage_overview_ra($people) {
  $new = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('person-new')),
    'data' => show_hide(t("Add a person"), add_person()),
  );

  return array(
    '#type' => 'container',
    '#attributes' => array('class' => array('manage-overview')),
    'new' => $new,
    'list' => persons_ra($people, 0, 'manage'),
  );
}


/***************
 Helpers
***************/
function add_person() {
  $person = new stdClass();
  $person->type = 'person';
  node_object_prepare($person);

  // Set some default fields: company ref and Employee type by default
  $person->field_company_ref = tf_em_get_company();
  $person->field_person_type = array('und' => array(array('value' => 2)));

  // TODO: Find source of bug http://drupal.org/node/1824820
  // It has something to do with the avatar that doesn't exist for a new person
  $person->field_avatar = get_default_avatar();

  module_load_include('inc', 'tf_manage', 'person_edit');
  return tf_manage_person_edit_ra($person, 'add');
}
