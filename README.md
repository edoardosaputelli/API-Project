# API - Project
Algoritmi e Principi dell'Informatica - Final examination - From the course held at PoliMi during the year 2018/2019

## Aim of the examination
Implementing in C a system capable of monitoring relationships between entities.  
The entities are supposed to change during time, through commands given on standard input.  
The user is allowed to add and delete the entities and the relationships, in addition to ask the system to show the relationships' list on standard output.  
  
The available commands are:
- addent ⟨id_ent⟩: to add the entity ⟨id_ent⟩;  
- delent ⟨id_ent⟩: to delete the entity ⟨id_ent⟩;  
- addrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩: to add the relationship ⟨id_rel⟩ between the entities ⟨id_orig⟩ and ⟨id_dest⟩;  
- delrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩: to delete the relationship ⟨id_rel⟩ between the entities ⟨id_orig⟩ and ⟨id_dest⟩;  
- report: to see on stdout the relationships' list and the entity with the biggest number of receiving relationships;
- end: to terminate the commands' sequence.
