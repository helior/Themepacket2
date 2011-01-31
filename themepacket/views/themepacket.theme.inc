<?php
// $Id$

/**
 * @file
 * Preprocessors for Themepacket Views Display Plugin.
 */

function template_preprocess_views_themepacket_view(&$variables, $hook) {
  // Envoke initial preprocessor.
  template_preprocess_views_view($variables);
  
  global $theme;
  $assets = assets_get_registry();
  
  if (!empty($assets->data)) {  

    $templates = $variables['view']->display_handler->compatible_templates();
    foreach ($templates as $key) {
      if (isset($assets->data[$key])) {        
        
        $item = $assets->data[$key];
        if (!empty($item['js'])) {
          foreach ($item['js'] as $js) {
            drupal_add_js($js->uri);
          }
        }
      
        if (!empty($item['css'])) {
          foreach ($item['css'] as $css) {
            drupal_add_css($css->uri);
          }
        }
      
        break;
      }
    }
  }

  // Display attributes
  $attributes = array();
  $attributes['class'] = implode(' ', $variables['classes_array']);
  
  $id = check_plain($variables['view']->display[$variables['view']->current_display]->display_options['css_id']);
  if (!empty($id)) {
    $attributes['id'] = $id;
  }
  $variables['views_attributes'] = drupal_attributes($attributes);
}


/**
 * Implementation of hook_preprocess_views_view_fields().
 */
function themepacket_preprocess_views_view_fields(&$variables, $hook) {
  //get view configuration
  $options = $variables['view']->display[$variables['view']->current_display]->display_options;
  
  //Prepare Preprocessed Fields for template
  if (isset($options['preprocess_fields']) && $options['preprocess_fields']) {
    $fields = $options['preprocessed'];
    if (is_array($fields)) {
      foreach ($fields as $original => $custom) {
        $variables[$custom] = $variables['fields'][$original]->content;
      }
    }
  }
}
