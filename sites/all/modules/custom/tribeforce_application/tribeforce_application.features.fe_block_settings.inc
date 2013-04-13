<?php
/**
 * @file
 * tribeforce_application.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function tribeforce_application_default_fe_block_settings() {
  $export = array();

  $export['version'] = '2.0';

  $export['menu-menu-dashboard'] = array(
    'cache' => -1,
    'custom' => '0',
    'delta' => 'menu-dashboard',
    'module' => 'menu',
    'node_types' => array(),
    'pages' => 'dashboard',
    'roles' => array(
      'company administrator' => '5',
    ),
    'themes' => array(
      'bartik' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'bartik',
        'weight' => '0',
      ),
      'tf_app' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'tf_app',
        'weight' => '-11',
      ),
      'zen' => array(
        'region' => '',
        'status' => 0,
        'theme' => 'zen',
        'weight' => '0',
      ),
    ),
    'title' => '<none>',
    'visibility' => '1',
  );

  return $export;
}
