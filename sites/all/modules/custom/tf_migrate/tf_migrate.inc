<?php
/*
 * @file
 * Classes to do the Roles Migration
 */
// TODO: Add Template fields and values, or cloned_from field


abstract class CommonMigration extends Migration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance('Role'));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'id' => array('type' => 'int',
                      'unsigned' => TRUE,
                      'not NULL' => TRUE,
                )
      ),
      MigrateDestinationNode::getKeySchema()
    );
    $this->addFieldMapping('language')->defaultValue('und');
  }
}

class TalentAttributeMigration extends CommonMigration {
  public function __construct() {
    parent::__construct();

    // The definition of the columns. Keys are integers, values are an
    // array of field name then description.
    $columns = array(
      0 => array('id', 'ID'),
      2 => array('title', 'Talent Attribute'),
      3 => array('body', 'Definition'),
      4 => array('question', 'Question'),
      6 => array('pos_answer', 'Positive Answer'),
      8 => array('neg_answer', 'Negative Answer'),
      10 => array('pos_beh', 'Positive Behaviour'),
      12 => array('neg_beh', 'Negative Behaviour'),
    );

    // Instantiate the class using the path to the CSV file and the columns.
    $this->source = new MigrateSourceCSV(
                        'sites/default/files/talent attributes.csv', $columns,
                        array('header_rows' => 1, 'embedded_newlines' => TRUE));
    $this->destination = new MigrateDestinationNode('talent_attribute');
    $this->addSimpleMappings(array('title', 'body'));
    $this->addFieldMapping('field_question', 'question');
    $this->addFieldMapping('field_pos_answer', 'pos_answer');
    $this->addFieldMapping('field_neg_answer', 'neg_answer');
    $this->addFieldMapping('field_pos_hint', 'pos_beh');
    $this->addFieldMapping('field_neg_hint', 'neg_beh');
  }
}

class TalentMigration extends CommonMigration {
  public function __construct() {
    parent::__construct();

    // The definition of the columns. Keys are integers, values are an
    // array of field name then description.
    $columns = array(
      0 => array('id', 'ID'),
      1 => array('ta_ids', 'TA IDs'),
      3 => array('cap_level', 'Capability Level'),
      4 => array('type', 'Talent Type'),
      5 => array('title', 'Talent Name'),
      6 => array('body', 'Description'),
    );

    // Instantiate the class using the path to the CSV file and the columns.
    $this->source = new MigrateSourceCSV(
                        'sites/default/files/talents.csv', $columns,
                        array('header_rows' => 1, 'embedded_newlines' => TRUE));
    $this->softDependencies = array('TalentAttribute');
    $this->destination = new MigrateDestinationNode('talent');
    $this->addSimpleMappings(array('title', 'body'));
    $this->addFieldMapping('field_cap_level', 'cap_level');
    $this->addFieldMapping('field_type', 'type');
    $this->addFieldMapping('field_talent_attr_ref', 'ta_ids')
      ->sourceMigration('TalentAttribute')
      ->separator(';');
  }
}


class TaskMigration extends CommonMigration {
  public function __construct() {
    parent::__construct();

    // The definition of the columns. Keys are integers, values are an
    // array of field name then description.
    $columns = array(
      0 => array('id', 'ID'),
      1 => array('title', 'Name'),
    );

    // Instantiate the class using the path to the CSV file and the columns.
    $this->source=new MigrateSourceCSV('sites/default/files/tasks.csv',$columns,
                        array('header_rows' => 1, 'embedded_newlines' => TRUE));
    $this->destination = new MigrateDestinationNode('task');
    $this->addFieldMapping('title', 'title');
  }
}

class SkillMigration extends CommonMigration {
  public function __construct() {
    parent::__construct();

    // The definition of the columns. Keys are integers, values are an
    // array of field name then description.
    $columns = array(
      0 => array('id', 'ID'),
      1 => array('title', 'Name'),
      2 => array('body', 'Description'),
      3 => array('type', 'Type'),
      4 => array('expert_level', 'Expert Level'),
    );

    // Instantiate the class using the path to the CSV file and the columns.
    $this->source=new MigrateSourceCSV(
      'sites/default/files/skills.csv',$columns, array('header_rows' => 1));
    $this->destination = new MigrateDestinationNode('skill');
    $this->addSimpleMappings(array('title', 'body'));
//    $this->addFieldMapping('field_cloned_from')->defaultValue(0);
    $this->addFieldMapping('field_skill_type', 'type');
    $this->addFieldMapping('field_expert_level', 'expert_level')
      ->separator(';');
  }
}

class RoleMigration extends CommonMigration {
  public function __construct() {
    parent::__construct();

    // The definition of the columns. Keys are integers, values are an
    // array of field name then description.
    $columns = array(
      0 => array('id', 'ID'),
      1 => array('title', 'Name'),
      2 => array('body', 'Description'),
      3 => array('cat', 'Category'),
      4 => array('task_ids', 'Task IDs'),
      5 => array('skill_ids', 'Skill IDs'),
      6 => array('talent_ids', 'Talent IDs'),
    );

    // Instantiate the class using the path to the CSV file and the columns.
    $this->source=new MigrateSourceCSV('sites/default/files/roles.csv',$columns,
                        array('header_rows' => 1, 'embedded_newlines' => TRUE));
    $this->softDependencies = array('Task', 'Skill', 'Talent');
    $this->destination = new MigrateDestinationNode('role');
    $this->addSimpleMappings(array('title', 'body'));
    $this->addFieldMapping('field_category', 'cat')
      ->separator(';');
    $this->addFieldMapping('field_task_ref', 'task_ids')
      ->sourceMigration('Task')
      ->separator(';');
    $this->addFieldMapping('field_skill_ref', 'skill_ids')
      ->sourceMigration('Skill')
      ->separator(';');
    $this->addFieldMapping('field_talent_ref', 'talent_ids')
      ->sourceMigration('Talent')
      ->separator(';');
  }
}
