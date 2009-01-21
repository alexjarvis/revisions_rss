<?php
// $Id: revisions_rss.admin.inc,v 1.1 2009/01/21 21:29:18 alexj Exp $

/**
 * @file
 * Administration callbacks for Revisions RSS.
 */

/**
 * Menu callback; displays the Revisions RSS admin settings page.
 */
function revision_rss_admin_settings() {
  $form['revision_rss_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Generate RSS for these node types'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('revision_rss_nodetypes', array('page')));
  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}