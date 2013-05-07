<?php
/**
 * @file
 * tribeforce_application.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function tribeforce_application_user_default_roles() {
  $roles = array();

  // Exported role: Feedback.
  $roles['Feedback'] = array(
    'name' => 'Feedback',
    'weight' => '7',
  );

  // Exported role: Recruit.
  $roles['Recruit'] = array(
    'name' => 'Recruit',
    'weight' => '9',
  );

  // Exported role: Roles.
  $roles['Roles'] = array(
    'name' => 'Roles',
    'weight' => '8',
  );

  // Exported role: Tribeforce Administrator.
  $roles['Tribeforce Administrator'] = array(
    'name' => 'Tribeforce Administrator',
    'weight' => '6',
  );

  // Exported role: Tribeforce Common.
  $roles['Tribeforce Common'] = array(
    'name' => 'Tribeforce Common',
    'weight' => '10',
  );

  return $roles;
}
