Drupal 7 get_entities_data
=================

The purpose of this project is to provide to  drupal programmers an easy way to access data located in drupal entities.

### Done
Single drupal node entity query

### Usage
Include this code in yout module
```bash
  module_load_include('module', 'get_entities_data', 'get_entities_data.module');
  //this is a example for call
  $ci = "12345";
  $partner = get_datas('node', 'persona', "nombres apellido_paterno apellido_materno ci", "ci=$ci");
```

### To do
Port to OO programing
Test and develop for not node entities
1-n relations
n-1 relations
1-1 relations
