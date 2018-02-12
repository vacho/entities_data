<?php
/**
 * @file
 * EntitiesData class.
 */

class EntitiesData {

  /**
   * Implements getDatas().
   * Return entities query.
   *
   * @param entity_type
   *   thats suppport node, user
   * @param bundle
   *   article for example
   * @param fields
   *  string of fields such as system name (lowercase) separated by single space " "
   *  eg (field_name_1 field_name_2 ... field_name_n)
   *  eg (field_name_1 ... entityName(entity_type,field_name_5 field_name_6 ...) ...field_name_n)
   * @param conditions
   *   string of conditions such as sql condition, separated by comma (,)
   */
  public static function getData($parameters) {

    $entity_type = $parameters['entity_type'];
    $bundle = $parameters['bundle'];
    $conditions = $parameters['conditions'];

    $query = new EntityFieldQuery();

    $field_conditions = ""; $entity_conditions = ""; $property_conditions = "";

    if($conditions != "") {
      $conditions = $parameters['conditions'];
      foreach ($conditions AS $condition) {
        $field = $condition['field'];
        $column = $condition['column'];
        $value = $condition['value'];
        $value_ini = $condition['value_ini'];
        $value_end = $condition['value_end'];
        $operator = $condition['operator'];
        $delta_group = $condition['delta_group'];
        $language_group = $condition['language_group'];

        if($condition['type'] == 'property_condition') {
          // ->propertyCondition($name, $value, $operator = NULL)
          if($operator == 'BETWEEN') {
            $property_conditions .= "->propertyCondition('$field', array($value_ini, $value_end), '$operator')";
          } else {
            $property_conditions .= "->propertyCondition('$field', '$value_ini$value$value_end', '$operator')";
          }
        }
        else if($condition['type'] == 'field_condition') {
          // ->fieldCondition($field, $column = NULL, $value = NULL, $operator = NULL, $delta_group = NULL, $language_group = NULL)
          $field_conditions .= "->fieldCondition('$field', '$column', '$value_ini$value$value_end', '$operator')";
        }
        else if($condition['type'] == 'entity_condition') {
          // ->entityCondition($name, $value, $operator = NULL)
          $entity_conditions .= "->entityCondition('$field', $value, '$operator')";
        }
      }
    }

    // GET all instances of entity_type, bundle for field conditions
    $composition = '
      $query
        ->entityCondition' . "('entity_type', '$entity_type')
        ->entityCondition('bundle', '$bundle')
        $property_conditions
        $field_conditions
        $entity_conditions
        ->execute();
    ";

    $entities = NULL;
    eval("\$entities=$composition;");

    if (sizeof($entities) > 0) {
      $entities = entity_load($entity_type, array_keys($entities[$entity_type]));

      // GET all field data
      foreach ($entities as $entity) {
        $field_data = array();

        $field_data['nid'] = $entity->nid;
        $field_data['vid'] = $entity->vid;
        $field_data['title'] = $entity->title;
        $field_data['created'] = $entity->created;
        $field_data['changed'] = $entity->changed;

        foreach ($parameters['fields'] AS $field) {

          $is_entity = is_array($field);
          $conditions_to_send = array();

          if ( $is_entity ) { // SUB ENTITY
            $entity_id = 0;
            $sss = '$entity->'. $field['field'] . "[und][0]['target_id']";
            eval("\$entity_id=$sss;");

            $conditions_to_send[] = array(
              'type' => 'entity_condition',
              'field' => 'entity_id',
              'value' => $entity_id,
              'operator' => '='
            );
            $params = array(
              'entity_type' => $field['entity_type'],
              'bundle' => $field['bundle'],
              'fields' => $field['fields'],
              'conditions' => $conditions_to_send
            );
            $field_name = $field['field'];

            $field_data["$field_name"] = self::getData($params);

          }
          else { // FIELD
            $field_name = $field;
            eval("\$field_data['$field_name']=" . '$entity->' . $field_name . "[LANGUAGE_NONE][0][value];");
          }
        }
        $data[] = $field_data;
      }

    }
    else {
      $data = NULL;
    }

    //dd($data);

    return $data;
  }

  /**
   * Implements setDatas().
   * Set datas in a entities.
   *
   * @param entity_type
   *   node, or other
   * @param entity_id
   *   id of entity
   * param fields
   *  string of fields such as system name (lowercase) separated by ","
   * @param fields_values
   *  string of conditions such as sql condition, separated by comma (,)
   *  if a field value is a copy of other field value set "field_" prefix
   */
  public static function setDatas($entity_type, $entity_id, $fields, $fields_values) {
    if($entity_type == "node") {
      $node = node_load($entity_id);

      $fields = explode(",", $fields);
      $fieldValues = explode(",", $fields_values);

      for ($i = 0; $i < count($fields); $i++) {

        $pos = strpos($fieldValues[$i], '_');

        if ($pos > 0) { // is a field
          $fieldValue = substr($fieldValues[$i], $pos+1, strlen($fieldValues[$i]));
          eval("\$node->" . $fields[$i] ."['und'][0]['value'] = \$node->" . $fieldValue . "['und'][0]['value'];");
        }else {
          eval("\$node->" . $fields[$i] ."['und'][0]['value'] = '" . $fieldValues[$i] . "';");
        }
      }
      node_save($node);
    }
  }
}
