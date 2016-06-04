<?php

/**
 * Dashboard, list all created rules
 */
function wysiwyg_source_formater_rules($form, &$form_state) {
  // TODO: Add operations column
  $header = array(
    'title' => array('data' => t('Title')),
    'rules' => array('data' => t('Rules')),
    'tags' => array('data' => t('Tags')),
  );

  $rows = wysiwyg_source_formater_retrieve_rules();

  $form['wysiwyg_source_formater_rules'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No rules available.'),
  );

  return $form;
}

/**
 * Return rules formated for the table
 */
function wysiwyg_source_formater_retrieve_rules() {
  $rows = array();
  $wsf_rules = entity_load('wsf');

  //TODO: Unserialize data and theme them
  foreach($wsf_rules as $wsf_rule) {
    $rows[] = array(
      'title' => $wsf_rule->title,
      'rules' => $wsf_rule->rules,
      'tags' => $wsf_rule->tags,
    );
  }
  return $rows;
}

// Form to add a new formating rule
function wysiwyg_source_formater_edit_rule($form, &$form_state, $wsf_id = NULL) {
  if($wsf_id) {
    $wsf = wysiwyg_source_formater_load($wsf_id);
  }

  // Title
  $title = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($wsf) ? $wsf->title : NULL,
  );

  // Rule
  $indent = array(
    '#type' => 'checkbox',
    '#title' => t('Indent'),
  );
  $breakBeforeOpen  = array(
    '#type' => 'checkbox',
    '#title' => t('Break before open'),
  );
  $breakAfterOpen  = array(
    '#type' => 'checkbox',
    '#title' => t('Break after open'),
  );
  $breakBeforeClose = array(
    '#type' => 'checkbox',
    '#title' => t('Break before close'),
  );
  $breakAfterClose = array(
    '#type' => 'checkbox',
    '#title' => t('Break after close'),
  );

  $rules = array(
    '#type' => 'fieldset',
    '#title' => t('Rules'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    'indent' => $indent,
    'breakBeforeOpen' => $breakBeforeOpen,
    'breakAfterOpen' => $breakAfterOpen,
    'breakBeforeClose' => $breakBeforeClose,
    'breakAfterClose' => $breakAfterClose,
  );

  // Tags
  $list_tags = array(
    'a',
    'abbr',
    'address',
    'area',
    'article',
    'aside',
    'audio',
    'b',
    'base',
    'bdi',
    'bdo',
    'blockquote',
    'body',
    'br',
    'button',
    'canvas',
    'caption',
    'cite',
    'code',
    'col',
    'colgroup',
    'datalist',
    'dd',
    'del',
    'details',
    'dfn',
    'dialog',
    'div',
    'dl',
    'dt',
    'em',
    'embed',
    'fieldset',
    'figcaption',
    'figure',
    'footer',
    'form',
    'h1',
    'h2',
    'h3',
    'h4',
    'h5',
    'h6',
    'head',
    'header',
    'hr',
    'html',
    'i',
    'iframe',
    'img',
    'input',
    'ins',
    'kbd',
    'keygen',
    'label',
    'legend',
    'li',
    'link',
    'main',
    'map',
    'mark',
    'menu',
    'menuitem',
    'meta',
    'meter',
    'nav',
    'noscript',
    'object',
    'ol',
    'optgroup',
    'option',
    'output',
    'p',
    'param',
    'pre',
    'progress',
    'q',
    'rp',
    'rt',
    'ruby',
    's',
    'samp',
    'script',
    'section',
    'select',
    'small',
    'source',
    'span',
    'strong',
    'style',
    'sub',
    'summary',
    'sup',
    'table',
    'tbody',
    'td',
    'textarea',
    'tfoot',
    'th',
    'thread',
    'time',
    'title',
    'tr',
    'track',
    'u',
    'ul',
    'var',
    'video',
    'wbr',
  );

  $tags = array(
    '#type' => 'fieldset',
    '#title' => t('Tags'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach($list_tags as $tag) {
    $tags[$tag] = array(
      '#type' => 'checkbox',
      '#title' => $tag,
    );
  }

  $other = array(
    '#title' => t('Other tags (one by line) :'),
    '#type' => 'textarea',
  );

  $tags['other'] = $other;

  //Submit button
  $submit = array(
    '#type' => 'submit',
    '#value' => isset($wsf) ? t('Update rule') : t('Add new rule'),
    '#submit' => isset($wsf) ? array('wysiwyg_source_formater_update_rule') : array('wysiwyg_source_formater_insert_rule'),
  );

  // The form
  $form = array(
    'title' => $title,
    'rules' => $rules,
    'tags' => $tags,
    'submit' => $submit,
  );

  return $form;
}

// Form submit : Update the rule
function wysiwyg_source_formater_update_rule($form, &$form_state) {
  // TODO
}

// Form insert : Update the rule
function wysiwyg_source_formater_insert_rule($form, &$form_state) {
  $wsf = array(
    'title' => '',
    'rules' => '',
    'tags' => '',
  );

  $data = array();

  // Get all fieldset names
  $groups = array();
  foreach($form_state['groups'] as $key => $group) {
    if($group['#group_exists']){
      $groups[] = $key;
    }
  }

  foreach($groups as $fieldset) {
    if(isset($form_state['complete form'][$fieldset])) {
      $elements = element_children($form_state['complete form'][$fieldset]);
      foreach($elements as $element) {
        if($form_state['values'][$element]) {
          $data[$fieldset][$element] = TRUE;
        }
      }
    }
  }

  $wsf['title'] = isset($form_state['values']['title']) ? $form_state['values']['title'] : NULL;
  $wsf['rules'] = isset($data['rules']) ? serialize($data['rules']) : NULL;
  $wsf['tags'] = isset($data['tags']) ? serialize($data['tags']) : NULL;

  db_insert('wsf_rules')
    ->fields($wsf)
    ->execute();
}