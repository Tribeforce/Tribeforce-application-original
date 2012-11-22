<?php

/*
 Forms
*/
function campaign_form($form, &$form_state, $node) {
  field_attach_form('node', $node, $form, $form_state);
  unset($form['field_role_ref']['#weight']);

  return array(
    'title' => array(
      '#type' => 'textfield',
      '#title' => t('Campaign name'),
      '#default_value' => $node->title,
    ),
    'role' => $form['field_role_ref'],
//    'date' => $form['field_date'],
    'submit' => array(
      '#type' => 'submit',
      '#value' => empty($node->nid) ?
                                    t('Create campaign'): t('Edit Campaign'),
      '#value' => t('Edit'),
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
function campaign($nid, $role_nid = 0) {
  // $nid is 0 for new campaigns
  if(empty($nid)) {
    $campaign = new stdClass();
    $campaign->type = 'campaign';
    $campaign->title = '';
    node_object_prepare($campaign);
    // Link the campaign to the users company
    $campaign->field_company_ref = tf_em_get_company();
  }
  else {
    $campaign = node_load($nid); // Returns FALSE if node doesn't exist yet
  }

  if(!empty($role_nid)) {
    $campaign->field_role_ref['und'][0]['nid'] = $role_nid;
  }

  $message = array(
    '#markup' => t('Here you can define the information of this campaign.'),
  );

  return recruit(array(
    'content' => campaign_ra($campaign, $role_nid),
    'message' => $message,
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

  $prefilled = ($role_nid > 0)? 'prefilled' : '';

  return array(
    '#type' => 'container',
    '#attributes' => array('class' => array(
      'campaign-details',
      $prefilled,
    )),
    'intro' => array('#markup' => 'A whole intro text',),
    'form' => drupal_get_form('campaign_form', $node),
  );
}


/*
Helpers
*/
