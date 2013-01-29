<?php
// Copyright (c) 2013 Femi Veys


/**
 * @file
 * tribeforce_application.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tribeforce_application_user_default_roles() {
  $roles = array();

  // Exported role: company administrator.
  $roles['company administrator'] = array(
    'name' => 'company administrator',
    'weight' => '4',
  );

  // Exported role: moderator.
  $roles['moderator'] = array(
    'name' => 'moderator',
    'weight' => '5',
  );

  // Exported role: super user.
  $roles['super user'] = array(
    'name' => 'super user',
    'weight' => '3',
  );

  return $roles;
}
