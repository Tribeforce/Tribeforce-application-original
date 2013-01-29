<?php
// Copyright (c) 2013 Femi Veys


/**
 * @file
 * tribeforce_application.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function tribeforce_application_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-dashboard:dashboard
  $menu_links['menu-dashboard:dashboard'] = array(
    'menu_name' => 'menu-dashboard',
    'link_path' => 'dashboard',
    'router_path' => 'dashboard',
    'link_title' => 'Dashboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: menu-dashboard:manage
  $menu_links['menu-dashboard:manage'] = array(
    'menu_name' => 'menu-dashboard',
    'link_path' => 'manage',
    'router_path' => 'manage',
    'link_title' => 'Manage',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-47',
  );
  // Exported menu link: menu-dashboard:recruit
  $menu_links['menu-dashboard:recruit'] = array(
    'menu_name' => 'menu-dashboard',
    'link_path' => 'recruit',
    'router_path' => 'recruit',
    'link_title' => 'Recruit',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: menu-dashboard:roles
  $menu_links['menu-dashboard:roles'] = array(
    'menu_name' => 'menu-dashboard',
    'link_path' => 'roles',
    'router_path' => 'roles',
    'link_title' => 'Roles',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Dashboard');
  t('Manage');
  t('Recruit');
  t('Roles');


  return $menu_links;
}
