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
  $details = array(
    'avatar' => field_view_field('node', $enriched_person['node'],
              'field_avatar', array(
                'label' => 'hidden',
                'weight' => -40,
                'settings' => array('image_style' => 'medium'))),
//    'remark' => drupal_get_form('tf_manage_remark_form', $enriched_person),
    'first_name' => field_view_field('node', $enriched_person['node'],
              'field_first_name', array('label' => 'hidden', 'weight' => -30)),
    'last_name' => field_view_title($enriched_person['node'], -20),
    'roles' => empty($enriched_person['roles']) ?
                        array() : tf_manage_roles_ra($enriched_person['roles']),
    'evaluations' => tf_manage_evaluations_ra($enriched_person),
    'edit' => array(
      '#theme' => 'link',
      '#text' => t('Edit details'),
      '#path' => 'manage/person/' . $enriched_person['node']->nid . "/edit",
      '#options' => array(
        'attributes' => array('class' => 'edit-link'),
        'html' => FALSE,
      ),
      '#weight' => 10,
    ),
  );

  $remarks = tf_manage_remarks_ra($enriched_person['remarks']);

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('person-details'),
    ),
    'data' =>   container_ra(array(
      'col-1' => $details,
      'col-2' => container_ra(array(
        'remarks' => show_hide(t('Remarks'), $remarks),
        'skills' => empty($enriched_person['skills']) ? array() :
             items_ra($enriched_person['skills'], 'skill', 0, 'person-details'),
        'talents' => empty($enriched_person['talents']) ? array() :
           items_ra($enriched_person['talents'], 'talent', 0, 'person-details'),
      )),
    )),
  );

}

function tf_manage_remarks_ra($remarks) {
  $ra = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('remarks'),
    ),
  );

  $index = 0;
  foreach($remarks as $remark) {
    $ra['nid-'. $remark->nid] = tf_manage_remark_ra($remark, $index);
    $index++;
  }

  return $ra;
}

function tf_manage_remark_ra($remark, $index) {
  $even_odd = $index % 2 == 0 ? 'even' : 'odd';

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'remark',
        'nid-' . $remark->nid,
        'index-' . $index,
        $even_odd,
      ),
    ),
    'remark' => field_view_title($remark),
    'date' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('date'),),
      'data' => array(
        '#markup' => format_date($remark->changed, 'short_date'),
      ),
    ),
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

function tf_manage_roles_ra($roles) {
  $ra = array(
    '#type' => 'container',
    '#weight' => -10,
    '#attributes' => array(
      'class' => array('roles'),
    ),
  );

  $index = 0;
  foreach($roles as $key => $role) {
    $ra[$key] = tf_manage_role_ra($role['node'], $index);
    $index++;
  }

  if($index == 0) {
    $ra['empty'] = container_ra(array(
      'empty' => array('#markup' => t('No role added yet'))
    ));
  }

  return $ra;
}


function tf_manage_role_ra($role, $index) {
   $even_odd = $index % 2 == 0 ? 'even' : 'odd';

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'role',
        'nid-' . $role->nid,
        'index-' . $index,
        $even_odd,
      ),
    ),
    '#theme' => 'link',
    '#text' => $role->title,
    '#path' => 'roles/' . $role->nid . '/view',
    '#options' => array(
      'attributes' => array(),
      'html' => FALSE,
    ),
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
