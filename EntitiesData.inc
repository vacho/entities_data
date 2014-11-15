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
    
    if($conditions != ""){
      
      $conditions = explode(",", $conditions);
      
      foreach ($conditions AS $condition) {
        $value = "value";
        
        if (strpos($condition, '!=') !== FALSE) {
          $elements = explode("!=", $condition);
          $operator =  "!=";
        }else if (strpos($condition, '>=') !== FALSE) {
          $elements = explode(">=", $condition);
          $operator =  ">=";
        }else if (strpos($condition, '<=') !== FALSE) {
          $elements = explode("<=", $condition);
          $operator =  "<=";
        }else if (strpos($condition, '>') !== FALSE) {
          $elements = explode(">", $condition);
          $operator =  ">";
        }else if (strpos($condition, '<') !== FALSE) {
          $elements = explode("<", $condition);
          $operator =  "<";
        }else if (strpos($condition, '=') !== FALSE) {
          $elements = explode("=", $condition);
          $operator =  "=";
        }
        $fieldName =  $elements[0];
        $fieldValue = $elements[1];
        
        if($elements[1] == 'NULL'){
          $value = 'value';
          $fieldConditions .= "->fieldCondition('field_$fieldName', '$value', '$fieldValue', '$operator')";
        }else {
          $fieldConditions .= "->fieldCondition('field_$fieldName', '$value', '$fieldValue', '$operator')";
        }
        
      }
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
        
        $fieldData['nid'] = $entity->vid;
        
        foreach ($fields AS $field) {
          $posIni = strpos($field, '('); 
          if ( $posIni > 0 ) { //entitie
            $posEnd = strpos($field, ')');
            $subFields = substr($field, $posIni+1, $posEnd-$posIni-1);
            $field = substr($field, 0, $posIni); 
          }
          
          eval("\$fieldData['$field']=" . '$entity->field_' . $field . "[LANGUAGE_NONE][0][value];");
          
          if ($fieldData["$field"] == NULL) {
            eval("\$fieldData['$field']=" . '$entity->field_' . $field . "[und];");
            
            $areEntities = FALSE;
            if ($fieldData["$field"] != NULL) { //Entities
              $areEntities = TRUE;
              $subFields = explode(",", $subFields);
              foreach ($fieldData["$field"] As $row) {
                $id = $row['target_id'];
                $type = $field;
                $object[$id] = (object)array( 'nid' => $id, 'vid' => $id, 'type' => "$type");
                $subEntity = entity_load($entity_type, array_keys($object));
                $subEntity = $subEntity[$id];
                
                foreach ($subFields AS $subField) {
                  eval("\$subFieldData['$subField']['$subEntity->title']=" . '$subEntity->field_' . $subField . "[LANGUAGE_NONE][0][value];");
                }
                
                $object =  array();
              }              

            }
            
            if ($areEntities) {
              $fieldData["$field"] = $subFieldData;
              $subFieldData = array();
            }
            
          }
        
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