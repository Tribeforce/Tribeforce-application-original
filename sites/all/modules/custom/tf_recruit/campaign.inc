<?php

/*
 Forms
*/
function campaign_form($form, &$form_state, $node, $prefilled) {
  field_attach_form('node', $node, $form, $form_state);
  unset($form['field_role_ref']['#weight']);

  if($prefilled) {
    $role = node_load($node->field_role_ref['und'][0]['nid']);
    $role_widget = container_ra(array('role' => $role->title));
  } else {
    if(isset($node->field_role_ref['und'][0]['nid'])) {
      $role = node_load($node->field_role_ref['und'][0]['nid']);
      $item = $role->title;
    } else {
      $item = t('Assign role');
    }
    $my_roles = tf_get_my_roles(); // TODO: cache this
    $dropdown = items_ra($my_roles, 'role', 0, 'widget');
    $role_widget = dropdown_widget_ra('', $item, $dropdown);
  }

  return array(
    'title' => array(
      '#type' => 'textfield',
      '#title' => t('Campaign name'),
      '#default_value' => $node->title,
      '#description' => t("We at Tribeforce like the name of a campaign and
                           season for easy remembering"),
      '#attributes' => array('placeholder' => 'example: User interface designer spring 2012'),
    ),
    'role' => $form['field_role_ref'],
//    'date' => $form['field_date'],
    'role-widget' => $role_widget,
    'actions' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('actions')),
      'submit' => array(
        '#type' => 'submit',
        '#attributes' => array('class' => array('btn', 'btn-primary')),
        '#value' => empty($node->nid) ?
                                      t('Create campaign'): t('Edit Campaign'),
//        '#value' => t('Edit'),
      ),
    ),
  );
}



/*
Form Submits
*/
function campaign_form_submit($form, &$form_state) {
  $campaign = $form_state['build_info']['args'][0];

  if(empty($campaign->nid)) { // New campaign
    $new_campaign = TRUE;
  }
  else { // Existing campaign
    $new_campaign = FALSE;
  }

  // Set the fields
  $campaign->title = $form_state['values']['title'];
  $campaign->field_role_ref = $form_state['values']['role'];

  // Save the new or updated node
  node_save($campaign);

  $next = 'recruit/' . $campaign->nid . '/prepare_interview';

  // Finally, go to the next step as created before
  drupal_goto($next);
}


/*
Page callbacks
*/
// If the $role_nid is not empty it propopulates the role widget
function campaign($campaign_nid, $role_nid = 0) {
  // $nid is 0 for new campaigns
  if(empty($campaign_nid)) {
    $campaign = new stdClass();
    $campaign->type = 'campaign';
    $campaign->title = '';
    node_object_prepare($campaign);
    // Link the campaign to the users company
    $campaign->field_company_ref = tf_em_get_company();
  }
  else {
    $campaign = node_load($campaign_nid); // Returns FALSE if node doesn't exist
  }

  if(!empty($role_nid)) {
    $campaign->field_role_ref['und'][0]['nid'] = $role_nid;
  }

  tf_help_msg(t('Here you can define basic information of this campaign.'));

  return recruit(array(
    'menu' => recruit_tabs_ra($campaign_nid),
    'content' => campaign_ra($campaign, $role_nid),
  ));
}


/*
AJAX callbacks
*/



/*
Render Arrays
*/
function campaign_ra($node, $role_nid = 0) {
  // TODO: Other text when $nid = 0 (=new)
  $prefilled = $role_nid > 0;

/*
  if($role_nid > 0) {
    $prefilled = 'prefilled';
    $role = node_load($role_nid);
    $role_widget = container_ra(array('role' => $role->title));
  } else {
    $prefilled = '';
    $item = t('Assign role');
    $my_roles = tf_get_my_roles(); // TODO: cache this
    $dropdown = items_ra($my_roles, 'role', $node->nid, 'widget');
    $role_widget = dropdown_widget_ra('', $item, $dropdown);
  }
*/

  $intro = t("Start by giving your recruiting campaign a name that works for you.<br />
              Then select the role you are hiring for. Recruiting with Tribeforce works best if you have focussed well on defining exactly which role you want to hire before you start.<br />
              If you want to edit your roles a bit more first, click on the <em>Roles</em> button on the top of the screen to go straight there and come back when you are ready. We always save your campaigns in their current state.");

  return array(
    '#type' => 'container',
    '#attributes' => array('class' => array(
      'campaign-details',
      $prefilled ? 'prefilled' : '',
    )),
    'intro' => container_ra(array('intro' => $intro)),
    'form' => drupal_get_form('campaign_form', $node, $prefilled),
  );
}


/*
Helpers
*/
