Drupal 7 entities_data
=================

The purpose of this project is to provide to  drupal programmers an easy way to access an save datas located in drupal entities.

### Done
* Queries to 1 Single node entity.

### Usage
Include this code in your module
```bash
  //examplecode
  $ci = "12345";
  $partner = get_datas('node', 'persona', "nombres apellido_paterno", "ci=$ci");
```

### To do
Queries for get:
* 1-n relations.
* n-1 relations.
* 1-1 relations.
* n-n relations.
Queries for set:
* 1-n relations.
* n-1 relations.
* 1-1 relations.
* n-n relations.
