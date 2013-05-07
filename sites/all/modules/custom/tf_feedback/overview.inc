<?php
// Copyright (c) 2013 Femi Veys


/******
 Forms
******/
function tf_feedback_remark_form($form, &$form_state, $node, $redirect) {
  $is_new = empty($node->nid);

  return array(
    'remark' => array(
//      '#title' => t('Remark'),
      '#type' => 'textarea',
      '#attributes' => array('placeholder' => t('Add a remark')),
      '#default_value' => isset($node->title) ? $node->title : '',
    ),
    'attached_to' => array(
      '#type' => 'hidden',
      '#default_value' => isset($node->field_attached_to['und'][0]['nid']) ?
                                 $node->field_attached_to['und'][0]['nid'] : '',
    ),
    'actions' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('actions')),
      'submit' => array(
        '#type' => 'submit',
        '#value' => $is_new ? t('Add') : t('Save'),
        '#attributes' => array('class' => array('btn', 'btn-primary')),
      ),
    ),
  );
}






/***************
 Form submits
***************/
function tf_feedback_remark_form_submit($form, &$form_state) {
  // Get the node from the build_info variables in the form_state
  $remark = $form_state['build_info']['args'][0];
  $redirect = $form_state['build_info']['args'][1];

  $remark_txt = trim($form_state['values']['remark']);
  $attached_to = $form_state['values']['attached_to'];

  // Populate the fields
  $remark->title = $remark_txt;
  if(empty($attached_to)) {
    if(isset($remark->field_attached_to)) unset($remark->field_attached_to);
  } else {
    $remark->field_attached_to['und'][0]['nid'] = $attached_to;
  }

  // Save the Remark node
  node_save($remark);

  $form_state['redirect'] = $redirect;
}


/***************
 Page callbacks
***************/
function feedback($regions = array()) {
  $my_people = get_enriched_persons();
  tf_help_msg(t('Hover over a person to add a remark or change details'));

  $empty_regions = array(
    'left-sidebar' => array(),
    'content' => feedback_overview_ra($my_people),
  );

  return tf_module('feedback', $empty_regions, $regions);
}


/***************
 AJAX callbacks
***************/
function edit_remark($person_nid, $redirect = 0, $remark_nid = FALSE) {
  $person = node_load($person_nid);
  $remark = node_load($remark_nid);
  $enriched_person = get_enriched_person(array('node' => $person));

  show_overlay(tf_feedback_edit_remark_ra($enriched_person, $redirect, $remark));
}

/***************
 Render Arrays
***************/
function feedback_overview_ra($people) {
  $persons_types = persons_ra($people, 0, 'feedback');

  // Translate the LOV
  $lov = list_allowed_values(field_info_field('field_person_type'));

  foreach($persons_types as $person_type => $persons) {
    $tabs[$person_type] = $lov[substr($person_type, 12)]; // person-type-
    $panes[$person_type] = $persons;
  }

  $tabs['new'] = t("Add a person");
  $panes['new'] = add_person();

  $tab_pane = tabs_widget_ra($tabs, $panes);

  return container_ra(array('feedback-overview' => $tab_pane));

}


function add_remark_ra($enriched_person) {
  return edit_remark_ra($enriched_person, 1);
}


function edit_remark_ra($enriched_person, $redirect = 0, $remark = FALSE) {
  if($remark === FALSE) {
    // Create a dummy remark with the person linked in
    $remark = new stdClass();
    $remark->type = 'remark';
    node_object_prepare($remark);

    // Populate the fields
    $remark->field_person['und'][0]['nid'] = $enriched_person['node']->nid;
  }


  if(isset($remark->field_attached_to['und'][0]['nid'])) {
    $attached_to = node_load($remark->field_attached_to['und'][0]['nid']);
    $attached_to_title = $attached_to->title;
  } else {
    $attached_to_title = '';
  }

  $dropdown = array(
    'none' => container_ra(array('none' => t('None'))) ,
    'skills' => empty($enriched_person['skills']) ? array() :
         items_ra($enriched_person['skills'], 'skill', 0, 'widget'),
    'tas' => empty($enriched_person['tas']) ? array() :
         items_ra($enriched_person['tas'], 'talent attribute', 0, 'widget'),
  );

  // TODO: Solve this in a proper way
  $redirect = $redirect == 0 ? 'feedback'
                             : "feedback/person/" . $enriched_person['node']->nid;

  return array(
    '#type' => 'container',
    '#attributes' => array('class' => array('edit-remark')),
    'form' => drupal_get_form('tf_feedback_remark_form', $remark, $redirect),
    'attached_to' => dropdown_widget_ra('attached-to', $attached_to_title,
                                                                     $dropdown),
  );
}


function tf_feedback_edit_remark_ra($enriched_person, $redirect=0, $remark=FALSE){
  $ra = array(
    'add_remark' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('add-remark')),
      'header' => array(
        '#type' => 'container',
        '#attributes' => array('class' => array('header')),
        'full-name' => container_ra(array(
          'full-name' => get_full_name($enriched_person['node'])
        )),
        'feedback' => array(
          '#theme' => 'link',
          '#type' => 'container',
          '#attributes' => array('class' => array('feedback')),
          '#text' => t('Feedback'),
          '#path' => 'feedback/person/' . $enriched_person['node']->nid,
          '#options' => array(
            'attributes' => array('class' => array(
  //            'btn',
  //            'btn-primary',
            )),
            'html' => FALSE,
          ),
        ),
        'close' => array(
          '#type' => 'container',
          '#attributes' => array('class' => array('close')),
          'name' => array(
            '#markup' => '&times',
          ),
        ),
      ),
      'body' => edit_remark_ra($enriched_person, $redirect, $remark),
    ),
  );

  return overlay_ra($ra);
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

  module_load_include('inc', 'tf_feedback', 'person_edit');
  return tf_feedback_person_edit_ra($person, 'add');
}
