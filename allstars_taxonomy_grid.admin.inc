<?php

function allstars_taxonomy_grid_admin_settings($form, &$form_state) {
  $content_types = node_type_get_types();
  $content_types_options = array();
  foreach ($content_types as $content_type) {
    $content_types_options[$content_type->type] = t($content_type->name);
  }
  $allstars_taxonomy_grid_content_type = variable_get('allstars_taxonomy_grid_content_type', FALSE);
  $allstars_taxonomy_grid_content_type = isset($content_types_options[$allstars_taxonomy_grid_content_type]) ?
    $allstars_taxonomy_grid_content_type : FALSE;

  $form['allstars_taxonomy_grid_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Set content type you would like to show'),
    '#options' => $content_types_options,
    '#default_value' => $allstars_taxonomy_grid_content_type,
    '#empty_option' => t('--Choose content type--'),
    '#empty_value' => FALSE,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'ajax_allstars_taxonomy_grid_admin_settings_callback',
      'wrapper' => 'allstars-taxonomy-grid-content-type-settings',
    ),
  );
  $form['content_type_settings'] = array(
    '#type' => 'hidden',
    '#title' => t('Set up fields and display modes'),
    '#prefix' => '<div id="allstars-taxonomy-grid-content-type-settings">',
    '#suffix' => '</div>',
  );

  // Show content type specific settings only if content type is already chosen
  // or we are going to choose it using ajax.
  $allstars_taxonomy_grid_content_type_candidate =
    (!empty($form_state['values']['allstars_taxonomy_grid_content_type'])) ?
      $form_state['values']['allstars_taxonomy_grid_content_type'] : FALSE
  ;
  // If content type was changed via ajax, then this variant has priority.
  $content_type = $allstars_taxonomy_grid_content_type_candidate ?
    $allstars_taxonomy_grid_content_type_candidate : $allstars_taxonomy_grid_content_type;
  if ($content_type) {
    $form['content_type_settings']['#type'] = 'fieldset';

    // Collect all taxonomy term fields options.
    $taxonomy_term_fields = array();
    $field_instances = field_info_instances('node', $content_type);
    foreach ($field_instances as $field_instance) {
      $field_info = field_info_field($field_instance['field_name']);
      if ($field_info['type'] == 'taxonomy_term_reference') {
        $taxonomy_term_fields[$field_instance['field_name']] = t($field_instance['label']);
      }
    }

    // Collect all view modes options.
    $node_info = entity_get_info('node');
    $view_modes = array();
    foreach ($node_info['view modes'] as $view_mode_name => $view_mode) {
      $view_modes[$view_mode_name] = t($view_mode['label']);
    }

    // Set default values here.
    $default_horizontal_field = variable_get('allstars_taxonomy_grid_horizontal_field', FALSE);
    $default_horizontal_field = isset($taxonomy_term_fields[$default_horizontal_field]) ? $default_horizontal_field : FALSE;
    $default_vertical_field = variable_get('allstars_taxonomy_grid_vertical_field', FALSE);
    $default_vertical_field = isset($taxonomy_term_fields[$default_vertical_field]) ? $default_vertical_field : FALSE;
    $default_grid_view_mode = variable_get('allstars_taxonomy_grid_grid_view_mode', FALSE);
    $default_grid_view_mode = isset($view_modes[$default_grid_view_mode]) ? $default_grid_view_mode : FALSE;
    $default_grid_block_view_mode = variable_get('allstars_taxonomy_grid_block_view_mode', FALSE);
    $default_grid_block_view_mode = isset($view_modes[$default_grid_block_view_mode]) ? $default_grid_block_view_mode : FALSE;

    // Reset default values if content type was changed via ajax.
    if ($allstars_taxonomy_grid_content_type_candidate) {
      // We should clear form state input here for select lists, because otherwise default values will not be applied.
      unset($form_state['input']['allstars_taxonomy_grid_horizontal_field']);
      unset($form_state['input']['allstars_taxonomy_grid_vertical_field']);
      unset($form_state['input']['allstars_taxonomy_grid_grid_view_mode']);
      unset($form_state['input']['allstars_taxonomy_grid_block_view_mode']);
      $default_horizontal_field = FALSE;
      $default_vertical_field = FALSE;
      $default_grid_view_mode = FALSE;
      $default_grid_block_view_mode = FALSE;
    }

    $form['content_type_settings']['allstars_taxonomy_grid_horizontal_field'] = array(
      '#type' => 'select',
      '#title' => t('Horizontal field'),
      '#options' => $taxonomy_term_fields,
      '#empty_option' => t('--Choose taxonomy field--'),
      '#empty_value' => FALSE,
      '#required' => TRUE,
      '#default_value' => $default_horizontal_field,
    );
    $form['content_type_settings']['allstars_taxonomy_grid_vertical_field'] = array(
      '#type' => 'select',
      '#title' => t('Vertical field'),
      '#options' => $taxonomy_term_fields,
      '#empty_option' => t('--Choose taxonomy field--'),
      '#empty_value' => FALSE,
      '#required' => TRUE,
      '#default_value' => $default_vertical_field,
    );
    $form['content_type_settings']['allstars_taxonomy_grid_grid_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('Grid view mode'),
      '#options' => $view_modes,
      '#empty_option' => t('--Choose view mode--'),
      '#empty_value' => FALSE,
      '#required' => TRUE,
      '#default_value' => $default_grid_view_mode,
    );
    $form['content_type_settings']['allstars_taxonomy_grid_block_view_mode'] = array(
      '#type' => 'select',
      '#title' => t('Block view mode'),
      '#options' => $view_modes,
      '#empty_option' => t('--Choose view mode--'),
      '#empty_value' => FALSE,
      '#required' => TRUE,
      '#default_value' => $default_grid_block_view_mode,
    );

  }
  $form['allstars_taxonomy_grid_block_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('jQuery selector for block'),
    '#default_value' => variable_get('allstars_taxonomy_grid_block_selector', ''),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Ajax callback for allstars_taxonomy_grid_admin_settings form.
 */
function ajax_allstars_taxonomy_grid_admin_settings_callback($form, $form_state) {
  return $form['content_type_settings'];
}
