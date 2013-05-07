<?php
/**
 * @file
 * tribeforce_application.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function tribeforce_application_user_default_permissions() {
  $permissions = array();

  // Exported permission: access tf_admin.
  $permissions['access tf_admin'] = array(
    'name' => 'access tf_admin',
    'roles' => array(
      0 => 'Tribeforce Administrator',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_common.
  $permissions['access tf_common'] = array(
    'name' => 'access tf_common',
    'roles' => array(
      0 => 'Tribeforce Common',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_feedback.
  $permissions['access tf_feedback'] = array(
    'name' => 'access tf_feedback',
    'roles' => array(
      0 => 'Feedback',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_recruit.
  $permissions['access tf_recruit'] = array(
    'name' => 'access tf_recruit',
    'roles' => array(
      0 => 'Recruit',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_roles.
  $permissions['access tf_roles'] = array(
    'name' => 'access tf_roles',
    'roles' => array(
      0 => 'Roles',
    ),
    'module' => 'tf_common',
  );

  return $permissions;
}
