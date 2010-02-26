<?php
// $Id: revisions_rss.admin.inc,v 1.3 2010/02/26 16:19:17 alexj Exp $

/**
 * @file
 * Administration callbacks for Revisions RSS.
 */

/**
 * Menu callback; displays the Revisions RSS admin settings page.
 */
function revisions_rss_admin_settings() {
  $form['revisions_rss_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Generate RSS for these node types'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('revisions_rss_nodetypes', array('page')));
  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}