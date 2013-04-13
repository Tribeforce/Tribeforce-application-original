<?php
/**
 * @file
 * tribeforce_application.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function tribeforce_application_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-dashboard.
  $menus['menu-dashboard'] = array(
    'menu_name' => 'menu-dashboard',
    'title' => 'Dashboard Menu',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard Menu');


  return $menus;
}
