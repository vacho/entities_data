Drupal 7 entities_data
=================

The purpose of this project is to provide to  drupal programmers an easy way to access an save datas located in drupal entities.

### Done
* Queries to 1 Single node entity.
* Queries to 1-1 nodes entities.
* Queries to 1-n nodes entities.


### Usage
Include this code in your module
```bash
  //examplecode
  $ci = "12345";
  $partner = EntitiesData::getDatas('node', 'persona', "nombres apellido_paterno", "ci=$ci");
  //this return a array of arrays
```

### To do
Queries for get:
* n-1 relations.
* n-n relations.

Queries for set:
* 1 relations.
* 1-1 relations.
* 1-n relations.
* n-1 relations.
* n-m relations.
