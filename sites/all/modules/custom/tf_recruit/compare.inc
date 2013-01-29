<?php
// Copyright (c) 2013 Femi Veys


/*
AJAX callbacks
*/

// $item_nid: item_nid (skill or ta)
// $candidate_nids_string: candidate_nids_string
function detail_pane($campaign_nid, $item_nid, $candidate_nids_string) {
  $role = node_load(get_role_nid_of_campaign($campaign_nid));
  $item = node_load($item_nid);
  $scores = get_scores_by_nids($role, explode(',', $candidate_nids_string));
  $output = drupal_render(
              compare_detail_pane_ra($item, $scores, $candidate_nids_string));
  $commands[] = ajax_command_replace('.detail-pane', $output);
  $commands[] = ajax_command_invoke(NULL, "invoke_unselect_candidates");

  ajax_deliver(array('#type' => 'ajax','#commands' => $commands));
}




/*
Render Arrays
*/
function compare_ra($campaign_nid, $role, $candidate_nids_string) {

  $scores = get_scores_by_nids($role, explode(',', $candidate_nids_string));

  return array(
    'page' => array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array(
          'compare',
          'clearfix',
        ),
      ),
      // TODO: Make generic for other than skills
      'score_board' => score_board_ra($campaign_nid, $scores, $candidate_nids_string),
      'detail_pane' => compare_detail_pane_ra(),
    ),
  );
}

function score_board_col_ra($val, $col_num, $key) {
  $even_odd = $col_num % 2 == 0 ? 'even' : 'odd';

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'col' ,
        'col-' . $col_num,
        'col-' . $even_odd,
        $key,
      ),
    ),
    'col' => array(
      '#markup' => $val,
    ),
  );
}

function score_board_row_ra($item_class, $scores, $row_num, $candidate_nids_string) {
  $exploded = explode('-', $item_class);
  $type = $exploded[0]; // is the type of the node
  $nid = $exploded[1]; // is the nid of the node (skill or ta)
  $candidate_nids = explode(',', $candidate_nids_string);

  $even_odd = $row_num % 2 == 0 ? 'even' : 'odd';

  // TODO: nojs implementation
  $ajax_url = "candidates/compare/ajax/detail_pane/$nid/$candidate_nids_string";

  $columns = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'columns',
        'columns-' . count($candidate_nids),
      ),
    ),
  );

  $counter = 1;
  foreach($candidate_nids as $candidate_nid) {
    $key = "candidate-$candidate_nid";
    $score = $scores[$key]; // Array holding the score object
                            // or NULL if no score has been profided
                            // $score['score'] holds the score as a number

    // If $score is NULL, it means no score has been set, we set N/A
    if(is_null($score['score'])) {
      $val = t('N/A');
    } else {
      $val = $score['score'];
    }
    $columns[$key] = score_board_col_ra($val, $counter, $key);
    $counter++;
  }

  // Add the title column
  $key = 'title';
  $node = node_load($nid);
  $columns[$key] = score_board_col_ra($node->title, $counter, $key);

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'clearfix',
        'row',
        'row-' . $row_num,
        'row-' . $even_odd,
        $type,
        $item_class,
      ),
    ),
    'link' => array(
      'columns' => $columns,
      '#prefix' => '<a href="' . $ajax_url . '" class="use-ajax">',
      '#suffix' => '</a>',
    ),
  );

}

function score_board_ra($campaign_nid, $scores_by_type, $candidate_nids_string) {
  $rows = array();
  $candidate_nids = explode(',', $candidate_nids_string);

  $even_odd = $row_num % 2 == 0 ? 'even' : 'odd';

  // TODO: nojs implementation
  $ajax_url = "candidates/compare/ajax/detail_pane/$nid/$candidate_nids_string";

  $counter = 1;
  // TODO: add type in the row
  foreach($scores_by_type as $type => $scores) {
    foreach($scores as $key => $score) { // $key is of format item-<nid>
      $rows[$key] = score_board_row_ra($key, $score, $counter, $candidate_nids_string);
      $counter++;
    }
  }


  // Header
  $header = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('header', 'clearfix'),
    ),
  );

  // Footer
  $footer = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('footer', 'clearfix'),
    ),
  );

  // Populate header and footer
  $counter = 1;
  foreach($candidate_nids as $candidate_nid) {
    $candidate = node_load($candidate_nid);

    $even_odd = $counter % 2 == 0 ? 'even' : 'odd';
    $attributes = array(
      'class' => array(
        'col',
        'col-' . $counter,
        'col-' . $even_odd,
        "candidate-" . $candidate_nid,
      ),
    );

    $header['col-'.$counter] = array(
      '#type' => 'container',
      '#attributes' => $attributes,
      'first-name' => field_view_field('node', $candidate, 'field_first_name',
                                   array('label' => 'hidden')),
      'last-name' => field_view_title($candidate, 10),
    );

    $footer['col-'.$counter] = array(
      '#type' => 'container',
      '#attributes' => $attributes,
      'checkbox' => array(
        '#type' => 'checkbox',
        '#attributes' => array(
          'checked' => 'checked',
          'id' => 'candidate-' . $candidate_nid,
          'name' => 'candidate-' . $candidate_nid,
        ),
      ),
      'hire'=> array(
        '#type' => 'container',
        '#attributes' => array('class' => array('hire-link')),
        '#theme' => 'link',
        '#text' => t('Hire'),
        '#path' => "recruit/$campaign_nid/candidates/hire/$candidate_nid",
        '#options' => array(
          'attributes' => array(),
          'html' => FALSE,
        ),
      ),
    );
    $counter++;
  }

  // Populate last column of header and footer
  $even_odd = $counter % 2 == 0 ? 'even' : 'odd';
  $attributes = array(
    'class' => array(
      'col',
      'col-' . $counter,
      'col-' . $even_odd,
    ),
  );

  $empty = array(
    '#type' => 'container',
    '#attributes' => $attributes,
  );


  $header['col-'.$counter] = $empty;
  $footer['col-'.$counter] = $empty;

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'score-board',
        'columns-' . count($candidate_nids),
      ),
    ),
    'header' => $header,
    'rows' => $rows,
    'footer' => $footer,
  );
}

function compare_detail_pane_ra($item = NULL,
                                $scores = array(),
                                $candidate_nids_string = '') {

  $empty_ra = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('title'),
    ),
    'title' => array('#markup' => t('Click on a row to compare.')),
  );

  return container_ra(array(
    'detail-pane' => container_ra(array(
      'ai' => empty($item) ? $empty_ra : compare_ai_ra($item),
      'score-details' => empty($scores) ? array() :
                    compare_scores_ra($item, $scores, $candidate_nids_string),
    )),
  ));
}

function compare_ai_ra($item) {
  return container_ra(array(
    'title' => field_view_title($item, 0),
    'body' => field_view_field('node', $item, 'field_body',
                                   array('label' => 'hidden', 'weight' => 10)),
    'pos-answer' => field_view_field('node', $item, 'field_pos_answer',
                                   array('weight' => 20)),
    'neg-answer' => field_view_field('node', $item, 'field_neg_answer',
                                   array('weight' => 30)),
  ));
}


function compare_scores_ra($item, $scores, $candidate_nids_string) {
  $ra = array();

  foreach($scores as $score) {
    $counter = 1;

    foreach(explode(',', $candidate_nids_string) as $candidate_nid) {
      $key = "candidate-$candidate_nid";
      $can_interpret = $score[$item->type . '-' . $item->nid][$key];

      if(!empty($can_interpret)) {
        $ra[$key] = compare_scores_row_ra($can_interpret, $counter, $key);
      }

      $counter++;
    }
  }

  return $ra;
}

function compare_scores_row_ra($can_interpret, $row_num, $key) {
  $even_odd = $row_num % 2 == 0 ? 'even' : 'odd';

  // $key holds the candidate nid. Example: candidate-1234
  $candidate = node_load(get_nid($key));

  return array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array(
        'clearfix',
        'row',
        'row-' . $row_num,
        'row-' . $even_odd,
        $key,
      ),
    ),
    'data' => container_ra(array(
      'name' => container_ra(array(
        'first-name' => field_view_field('node', $candidate, 'field_first_name',
                                     array('label' => 'hidden')),
        'last-name' => field_view_title($candidate, 10),
      )),
      'date' => array(
        '#markup'=> format_date($can_interpret['changed'],'short_date'),
      ),
      'score' => array(
        '#markup' => $can_interpret['score'],
      ),
      'comment' => array(
        '#markup' => $can_interpret['remark'],
      ),
    )),
  );

}

/*
Helpers
*/
