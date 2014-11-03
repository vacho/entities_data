<?php
/**
 * @file
 * EntitiesData class.
  */

class EntitiesData {
  
   /**
   * Implements get_datas().
   * Return entites query.
   *
   * @param entity_type
   *   node, or other 
   * @param bundle
   *   article or other
   * @param fields
   *   string of fields such as human name (lowercase) separated by single space ( )
   * @param conditios
   *   string of conditions such as sql condition, separated by comma (,)
   */
  public static function getDatas($entity_type, $bundle, $fields, $conditions) {
    $query = new EntityFieldQuery();
    
    $conditions = explode(",", $conditions);
    
    foreach ($conditions AS $condition) {
      if (strpos($condition, '=') !== FALSE) {
        $elements = explode("=", $condition);
        $operator =  "=";
      }
      
      $fieldName =  $elements[0];
      $fieldValue =  $elements[1];
      
      $fieldConditions .= "->fieldCondition('field_$fieldName', 'value', '$fieldValue', '$operator')";
    }
    
    $composition = '$query->entityCondition' . "('entity_type', '$entity_type')
    ->entityCondition('bundle', '$bundle')
    $fieldConditions
    ->execute();
    ";
    
    eval("\$entitiesInstances=$composition;");
    
    if (sizeof($entitiesInstances) > 0) {
      $entities = entity_load($entity_type, array_keys($entitiesInstances[$entity_type]));
      
      $fields = explode(" ", $fields);
      foreach ($entities as $entity) {
        $fieldData = array();
        foreach ($fields AS $field) {
          eval("\$fieldData['$field']=" . '$entity->field_' . $field . "[LANGUAGE_NONE][0][value];");
        }
        $datas[] = $fieldData;
      }
    
    }
    else {
      $datas = NULL;
    }
      
    return $datas;
  }

}
