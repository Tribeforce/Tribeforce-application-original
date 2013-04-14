<?php
/**
 * @file
 * tribeforce_application.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function tribeforce_application_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "corresponding_node_references" && $api == "default_corresponding_node_references_presets") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function tribeforce_application_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function tribeforce_application_image_default_styles() {
  $styles = array();

  // Exported image style: icon.
  $styles['icon'] = array(
    'name' => 'icon',
    'effects' => array(
      2 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '32',
          'height' => '32',
        ),
        'weight' => '1',
      ),
    ),
  );

  // Exported image style: overview_square.
  $styles['overview_square'] = array(
    'name' => 'overview_square',
    'effects' => array(
      1 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '150',
          'height' => '150',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function tribeforce_application_node_info() {
  $items = array(
    'campaign' => array(
      'name' => t('Campaign'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'company' => array(
      'name' => t('Company'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'evaluation' => array(
      'name' => t('Evaluation'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'interpretation' => array(
      'name' => t('Interpretation'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'interview' => array(
      'name' => t('Interview'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'remark' => array(
      'name' => t('Remark'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Remark'),
      'help' => '',
    ),
    'role' => array(
      'name' => t('Role'),
      'base' => 'node_content',
      'description' => t('Definition of a Role. Subsists of mainly:
- Tasks
- Skills
- Competencies'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'skill' => array(
      'name' => t('Skill'),
      'base' => 'node_content',
      'description' => t('A skill is something you are able to DO.
Example: being able to make a Drupal application'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'talent' => array(
      'name' => t('Talent'),
      'base' => 'node_content',
      'description' => t('TODO: Definition Talent'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'talent_attribute' => array(
      'name' => t('Talent Attribute'),
      'base' => 'node_content',
      'description' => t('Competencies can have several attributes. This content type holds the information constituting  these attributes.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'task' => array(
      'name' => t('Task'),
      'base' => 'node_content',
      'description' => t('A Task is what is expected from a candidate and is added to a Role'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
