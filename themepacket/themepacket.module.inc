<?php

/**
 * @file
 *  All the heavy lifting of the module is kept here since themepacket does 95%
 *  of its work when Drupal is rebuilding the theme registry, and we really
 *  don't need all this code loaded on every page request.
 */


function _themepacket($existing, $type, $theme, $path, $settings) {

  // Collect the interface that Themepacket will use to build theme hooks.
  $info = themepacket_get_info();
  if (!empty($settings)) {
    $info = drupal_array_merge_deep($info, $settings);
  }
  
  // Discover the path to all themepackets
  if (isset($info['settings']['theme path']) && !empty($info['settings']['theme path'])) {
    $path = $path .= '/' . $info['settings']['theme path'];
  }
  
  // Prepare $mask to find markup-type files. These will determine the base of
  // the themepackets.

  // $mask regex is a string beginning with one or more non-whitespace
  // characters, followed by a dot, followed by any extension registered by
  // hook_themepacket_info() that is of type 'include' or 'template'.
  $mask = '/^\S+\.(' . implode('|', $info['helpers']['markup_types']) . ')$/';
  
  // @todo provide opportunity to add configurable regex for 'nomask' @see file_scan_directory()
  $options = array();
  
  $files = file_scan_directory($path, $mask, $options);
  _themepacket_process_filenames($files);
  
  dpm($files);
  
  return array();
}


/**
 * Helper function to bisect multiple items and merge that data with each
 * element in the $files array.
 * 
 * @see themepacket_parse_filename()
 * @see file_scan_directory()
 * 
 * @param $files
 *   An array of file objects obtained from file_scan_directory.
 */
function _themepacket_process_filenames(&$files) {
  foreach ($files as $key => $file) {
    
    // This key isn't very useful so we remove it for the sake of keeping our
    // data as simple as possible.
    unset($files[$key]->name);
    
    $data = themepacket_parse_filename($file->filename);
    
    // Derive the path of the file
    $data['path'] = str_replace('/' . $file->filename, '', $file->uri);
    
    $files[$key] = array_merge($data, (array)$file);
  }
}
