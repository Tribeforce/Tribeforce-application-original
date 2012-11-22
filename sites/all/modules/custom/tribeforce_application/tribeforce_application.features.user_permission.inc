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

  // Exported permission: access tf_manage.
  $permissions['access tf_manage'] = array(
    'name' => 'access tf_manage',
    'roles' => array(
      0 => 'company administrator',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_recruit.
  $permissions['access tf_recruit'] = array(
    'name' => 'access tf_recruit',
    'roles' => array(
      0 => 'company administrator',
    ),
    'module' => 'tf_common',
  );

  // Exported permission: access tf_roles.
  $permissions['access tf_roles'] = array(
    'name' => 'access tf_roles',
    'roles' => array(
      0 => 'company administrator',
    ),
    'module' => 'tf_common',
  );

  return $permissions;
}
