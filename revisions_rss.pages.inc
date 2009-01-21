<?php
// $Id: revisions_rss.pages.inc,v 1.1 2009/01/21 21:29:18 alexj Exp $ 

/**
 * @file
 * Revisions RSS feed generator.
 */

/**
 * Menu callback; publish an RSS feed
 */
function revisions_rss_handler(&$node) {
  if ($output = revisions_rss_node_feed($node->nid)) {
      print $output;
      
      return;
  }
  
  drupal_not_found();
}

/**
 * Publish a feed for all revisions of a specific node
 *
 * @return bool True if successful in generating a feed, false otherwise.
 */
function revisions_rss_node_feed($nid) {
  global $base_url;

  $node = node_load($nid);

  if (revision_rss_check_type($node)) {
    $items = revisions_rss_get_revisions($node->nid);

    $namespaces = array();

    $channel = array(
      'title' => t('@site - Updates for "@title"', array(
        '@site' => variable_get('site_name', 'Drupal'),
        '@title' => $node->title)),
      'description' => t('Updates for "@title"', array('@title' => $node->title)),
      'link' => url('node/{$node->nid}', array('absolute' => TRUE)),
    );

    return revisions_rss_format_feed($items, $channel, $namespaces);
  }
  else {
    return FALSE;
  }
}

/**
 *  Format and print a revision feed.
 *
 * @param $items string The individual RSS items.
 * @param $channel array The channels present in the RSS.
 * @param $namespaces array The namespaces present in the RSS.
 */
function revisions_rss_format_feed($items, $channel = array(), $namespaces = array()) {
  global $base_url, $locale;

  $channel_defaults = array(
    'version' => '2.0',
    'title' => t('@site_name - Updates', array('@site_name' => variable_get('site_name', 'Drupal'))),
    'link' => $base_url,
    'description' => t('Updates'),
    'language' => $locale);
  $channel = array_merge($channel_defaults, $channel);
  $namespaces = array_merge(array('xmlns:dc="http://purl.org/dc/elements/1.1/"'), $namespaces);

  $output = '<?xml version="1.0" encoding="utf-8"?>'."\n";
  $output .= '<rss version="'. $channel['version'] .'" xml:base="'. $base_url .'" '. implode(' ', $namespaces) .">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
  $output .= "</rss>\n";

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');

  return $output;
}

/**
 *  Takes a node ID and generates the rss for that nodes revisions.
 *
 * @param $nid int The node ID of the note to generate RSS for.
 * @return The generated RSS.
 */
function revisions_rss_get_revisions($nid) {
  global $base_url;

  $items = '';

  $sql = "SELECT nr.nid
               , n.title
               , nr.uid
               , nr.vid
               , n.vid AS latest
               , nr.log
               , nr.timestamp
          FROM {node_revisions} nr
          INNER JOIN {node} n
             ON nr.nid = n.nid
          WHERE nr.nid = '$nid'
          ORDER BY nr.timestamp DESC";

  $nodes = db_query_range(db_rewrite_sql($sql), 0, variable_get('feed_default_items', 10));

  while ($node = db_fetch_object($nodes)) {
    $user = user_load(array('uid' => $node->uid));
    $node->name = $user->name;

    if (!empty( $node->log)) {
        $node->description = "Log: {$node->log}\n\n";
    }

    $node->description .= "Updated by: {$user->name}\n\n";

    $extra = array(
      array(
        'key' => 'pubDate',
        'value' => date('r', $node->timestamp)),
      array(
        'key' => 'dc:creator',
        'value' => $node->name),
      array(
        'key' => 'guid',
        'value' => t("Update !vid at !base/node/!nid", array('!vid' => $node->vid,
                                                             '!base' => $base_url,
                                                             '!nid' => $node->nid)),
        'attributes' => array('isPermaLink' => 'false'))
    );

    if (module_exists('diff') && $node->vid != $node->latest) {
      $node->description .= l("Diff vs Newest", "node/{$node->nid}/revisions/view/{$node->vid}/{$node->latest}");
    }

    $link = url("node/{$node->nid}", array('absolute' => TRUE));

    $items .= format_rss_item($node->title, $link, check_markup($node->description, $node->format, FALSE), $extra);
  }

  return $items;
}