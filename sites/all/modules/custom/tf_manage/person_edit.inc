<?php
// Copyright (c) 2013 Femi Veys


/***************
 Forms
***************/
function tf_manage_person_form($form, &$form_state, $node,
                               $display = 'details', $extra = array()){
  $is_new = empty($node->nid);

  // Get the form elements of the node
  field_attach_form('node', $node, $form, $form_state, 'und');

  $my_form['field_first_name'] = $form['field_first_name'];
  $my_form['title'] = array(
    '#title' => t('Last name'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $is_new ? '' : $node->title,
    '#weight' => 2,
  );

  $my_form['field_email'] = $form['field_email'];
  $my_form['field_cv'] = $form['field_cv'];
  $my_form['field_motivation'] = $form['field_motivation'];
  $my_form['field_avatar'] = $form['field_avatar'];
  $my_form['field_phone'] = $form['field_phone'];

  if($display === 'details' or $display === 'add' ) {
    $my_form['field_person_type'] = $form['field_person_type'];
    $my_form['field_hire_date'] = $form['field_hire_date'];
    $my_form['field_birth_date'] = $form['field_birth_date'];
  }

  $my_form['actions'] = array(
    // We assume the submit button is always last
    'cancel' => array(
      '#theme' => 'link',
      '#text' => t('Cancel'),
      '#options' => array(
        'attributes' => array('class' => array('cancel', 'btn')),
        'html' => FALSE,
      ),
    ),
    '#type' => 'container',
    '#attributes' => array('class' => array('actions')),
    '#weight' => 99,
    'submit' => array(
      '#type' => 'submit',
      '#value' => $is_new ? t('Add') : t('Save'),
      '#attributes' => array('class' => array('btn', 'btn-primary')),
    ),
  );

  // Change the actions by display
  switch($display) {
    case 'details':
      $my_form['actions']['cancel']['#path'] = 'manage/person/' . $node->nid;
      break;
    case 'candidate':
      if(empty($node->nid)) { // New person
        unset($my_form['actions']['cancel']);
//        $my_form['actions']['cancel']['#path'] = current_path();
      } else {
        $my_form['actions']['cancel']['#path'] = dirname(dirname(current_path()));
      }
      break;
    case 'add':
      $my_form['actions']['cancel']['#path'] = 'manage';
      break;
  }

  return $my_form;
}


/***************
 Form submits
***************/
function tf_manage_person_form_submit($form, &$form_state) {
  $person = $form_state['build_info']['args'][0];
  $display = $form_state['build_info']['args'][1];
  $extra = $form_state['build_info']['args'][2];
  $person->title = $form_state['values']['title'];
  $person->field_first_name = $form_state['values']['field_first_name'];
  $person->field_email = $form_state['values']['field_email'];
  $person->field_phone = $form_state['values']['field_phone'];

  if($display === 'details' or $display === 'add' ) {
    $person->field_person_type = $form_state['values']['field_person_type'];
    $person->field_hire_date = $form_state['values']['field_hire_date'];
    $person->field_birth_date = $form_state['values']['field_birth_date'];
  }

  // Handle the file uploads
  foreach(array('avatar', 'cv', 'motivation') as $file) {
    $property = "field_$file";

    // Check if the property was in the form
    if(!empty($form_state['values'][$property])) {
      // Check if a file has been uploaded
      if($form_state['values'][$property]['und'][0]['fid'] != 0)
        $person->$property = $form_state['values'][$property];
      else
        unset($person->$property);
    }

  }

  node_save($person);

  switch($display) {
    case 'details':
      $form_state['redirect'] = 'manage/person/' . $person->nid;
      break;
    case 'candidate':
      $campaign_nid = $extra['original_nid'];
      $form_state['redirect'] = "recruit/$campaign_nid/candidates";
      break;
    case 'add':
      $form_state['redirect'] = 'manage';
      break;
  }
}



/***************
 Page callbacks
***************/
function person_edit($nid) {
  $regions = array();
  $node = node_load($nid);
  $enriched_person = get_enriched_person(array('node' => $node));

  tf_help_msg(t('Change all personal details here'));
  $empty_regions = array(
    'left-sidebar' => array(),
    'content' => tf_manage_person_edit_ra($enriched_person['node'], 'details'),
  );

  return tf_module('manage', $empty_regions, $regions);
}



/***************
 Render Arrays
***************/
function tf_manage_person_edit_ra($person, $display = 'details', $extra = array()) {
  // TODO: This splitup is done to work around http://drupal.org/node/1824820
  // TODO: Probably this splitup is wrong. The existance of a field should be checked
  switch($display) {
    case 'details':
      $image = field_view_field('node', $person, 'field_avatar', array(
                  'label' => 'hidden',
                  'weight' => 0,
                  'settings' => array('image_style' => 'medium'),
               ));
      break;
    case 'add':
    case 'candidate':
      $image = array(
        '#type' => 'container',
        '#attributes' => array('class' => array(
          'field',
          'field-name-field-avatar',
          'field-type-image',
        )),
        'data' => array(
          '#markup' => theme_image(array(
            // This path should exist, but will be replaced by jQuery
            'path' => '/sites/default/files/person.png',

            'attributes' => array(),
          )),
        ),
      );
      break;
  }

  // There are problems with include files. That's why we need to build the form
  // in this way as per http://drupal.org/node/1336212#comment-6056524
  $form_state['build_info']['args'][] = $person;
  $form_state['build_info']['args'][] = $display;
  $form_state['build_info']['args'][] = $extra;
  form_load_include($form_state, 'inc', 'tf_manage', 'person_edit');
  $form = drupal_build_form('tf_manage_person_form', $form_state);

  // Get the roles data if the person exists
  if(isset($person->nid)) {
    $enriched_person = get_enriched_person(array('node' => $person));
    $empty_roles = container_ra(array('empty' => array(
      '#markup' => t('No role added yet')
    )));

    $roles = empty($enriched_person['roles']) ? $empty_roles :
       items_ra($enriched_person['roles'], 'role', $person->nid, 'person-edit');

    $item = t('Assign role');
    $my_roles = tf_get_my_roles(); // TODO: cache this
    $dropdown = items_ra($my_roles, 'role', $person->nid, 'widget_old');
    $assign_role = dropdown_widget_ra('', $item, $dropdown);
  } else {
    $roles = $assign_role = array();
  }

  $ra = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('person-edit', 'clearfix'),
    ),
    'data' => container_ra(array(
      'col-1' => container_ra(array(
        'avatar' => container_ra(array(
          'image' => $image,
          'remove' => t('Remove'),
          'add' => t('Add'),
          'edit' => t('Change'),
        )),
      )),
      'col-2' => $form,
      'col-3' => container_ra(array(
        'cv' => container_ra(array(
          'file-defined' => t('CV'),
          'file-removed' => t('No CV'),
          'remove' => t('Remove CV'),
          'add' => t('Add CV'),
        )),
        'motivation' => container_ra(array(
          'file-defined' => t('Motivation letter'),
          'file-removed' => t('No Motivation letter'),
          'remove' => t('Remove Motivation Letter'),
          'add' => t('Add Motivation Letter'),
        )),
        'roles' => $roles,
        'assign-role' => $assign_role,

      )),
    )),
  );

  return $ra;
}
