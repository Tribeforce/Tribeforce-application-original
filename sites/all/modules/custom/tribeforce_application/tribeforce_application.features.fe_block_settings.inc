<?php
// Copyright (c) 2013 Femi Veys


/**
 * @file
 * tribeforce_application.features.fe_block_settings.inc
 */

/**
 * Implements hook_default_fe_block_settings().
 */
function tribeforce_application_default_fe_block_settings() {
  $export = array();

  $export['version'] = '1.0';

  // Theme independent visibility settings.
  $export['visibility'] = array(
    'menu-menu-dashboard' => array(
      'delta' => 'menu-dashboard',
      'module' => 'menu',
    ),
  );

  // Exported block settings for Bartik.
  $theme = array();

  $theme['menu-menu-dashboard'] = array(
    'module' => 'menu',
    'delta' => 'menu-dashboard',
    'theme' => 'bartik',
    'status' => 0,
    'weight' => '0',
    'region' => '',
    'custom' => '0',
    'visibility' => '1',
    'pages' => 'dashboard',
    'title' => '<none>',
    'cache' => -1,
  );

  $export['theme']['bartik'] = $theme;

  // Exported block settings for Tribeforce application theme.
  $theme = array();

  $theme['menu-menu-dashboard'] = array(
    'module' => 'menu',
    'delta' => 'menu-dashboard',
    'theme' => 'tf_app',
    'status' => 0,
    'weight' => '-11',
    'region' => '',
    'custom' => '0',
    'visibility' => '1',
    'pages' => 'dashboard',
    'title' => '<none>',
    'cache' => -1,
  );

  $export['theme']['tf_app'] = $theme;

  // Exported block settings for Zen.
  $theme = array();

  $theme['menu-menu-dashboard'] = array(
    'module' => 'menu',
    'delta' => 'menu-dashboard',
    'theme' => 'zen',
    'status' => 0,
    'weight' => '0',
    'region' => '',
    'custom' => '0',
    'visibility' => '1',
    'pages' => 'dashboard',
    'title' => '<none>',
    'cache' => -1,
  );

  $export['theme']['zen'] = $theme;

  $theme_default = variable_get('theme_default', 'bartik');
  $themes = list_themes();
  foreach ($export['theme'] as $theme_key => $settings) {
    if ($theme_key != $theme_default && empty($themes[$theme_key]->status)) {
      unset($export['theme'][$theme_key]);
    }
  }
  return $export;
}
