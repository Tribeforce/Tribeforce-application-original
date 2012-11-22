<?php
/**
 * @file
 * tribeforce_application.default_corresponding_node_references_presets.inc
 */

/**
 * Implements hook_default_corresponding_node_references().
 */
function tribeforce_application_default_corresponding_node_references() {
  $export = array();

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'campaign*field_persons_back_ref*person*field_campaigns_ref';
  $cnr_obj->enabled = 1;
  $export['campaign*field_persons_back_ref*person*field_campaigns_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interpretation*field_ass_item_ref*skill*field_interpret_back_ref';
  $cnr_obj->enabled = 1;
  $export['interpretation*field_ass_item_ref*skill*field_interpret_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interpretation*field_ass_item_ref*talent_attribute*field_interpret_back_ref';
  $cnr_obj->enabled = 1;
  $export['interpretation*field_ass_item_ref*talent_attribute*field_interpret_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interpretation*field_parent_ref*evaluation*field_interprets_ref';
  $cnr_obj->enabled = 1;
  $export['interpretation*field_parent_ref*evaluation*field_interprets_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interpretation*field_parent_ref*interview*field_interprets_ref';
  $cnr_obj->enabled = 1;
  $export['interpretation*field_parent_ref*interview*field_interprets_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interview*field_campaign_ref*campaign*field_interviews_ref';
  $cnr_obj->enabled = 1;
  $export['interview*field_campaign_ref*campaign*field_interviews_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'interview*field_person_back_ref*person*field_assessments_ref';
  $cnr_obj->enabled = 1;
  $export['interview*field_person_back_ref*person*field_assessments_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'job_opening*field_company_ref*company*field_openings_back_ref';
  $cnr_obj->enabled = 1;
  $export['job_opening*field_company_ref*company*field_openings_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'job_opening*field_persons_back_ref*person*field_openings_ref';
  $cnr_obj->enabled = 1;
  $export['job_opening*field_persons_back_ref*person*field_openings_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'job_opening*field_role_ref*role*field_openings_back_ref';
  $cnr_obj->enabled = 1;
  $export['job_opening*field_role_ref*role*field_openings_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'person*field_company_ref*company*field_persons_back_ref';
  $cnr_obj->enabled = 1;
  $export['person*field_company_ref*company*field_persons_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'person*field_evaluations_ref*evaluation*field_person_back_ref';
  $cnr_obj->enabled = 1;
  $export['person*field_evaluations_ref*evaluation*field_person_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'remark*field_person*person*field_remarks_ref';
  $cnr_obj->enabled = 1;
  $export['remark*field_person*person*field_remarks_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'role*field_cloned_from*role*field_cloned_from';
  $cnr_obj->enabled = 0;
  $export['role*field_cloned_from*role*field_cloned_from'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'role*field_company_ref*company*field_roles_back_ref';
  $cnr_obj->enabled = 1;
  $export['role*field_company_ref*company*field_roles_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'role*field_persons_back_ref*person*field_role_ref';
  $cnr_obj->enabled = 1;
  $export['role*field_persons_back_ref*person*field_role_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'skill*field_role_back_ref*role*field_skill_ref';
  $cnr_obj->enabled = 1;
  $export['skill*field_role_back_ref*role*field_skill_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'talent*field_role_back_ref*role*field_talent_ref';
  $cnr_obj->enabled = 1;
  $export['talent*field_role_back_ref*role*field_talent_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'talent*field_talent_attr_ref*talent_attribute*field_talent_back_ref';
  $cnr_obj->enabled = 1;
  $export['talent*field_talent_attr_ref*talent_attribute*field_talent_back_ref'] = $cnr_obj;

  $cnr_obj = new stdClass();
  $cnr_obj->disabled = FALSE; /* Edit this to true to make a default cnr_obj disabled initially */
  $cnr_obj->api_version = 1;
  $cnr_obj->node_types_content_fields = 'task*field_role_back_ref*role*field_task_ref';
  $cnr_obj->enabled = 1;
  $export['task*field_role_back_ref*role*field_task_ref'] = $cnr_obj;

  return $export;
}
