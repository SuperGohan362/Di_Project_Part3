#include "Query_Executor.h"
#include "Projection_Executor/Projection_Executor.h"
#include "../Preparator/Preparator.h"



void Execute_Query(Query_Ptr Query, Table_Ptr Table, FILE *fp, uint64_t **Results_array, int query_id){
  printf("EXECUTE QUERY %d...\n", query_id);

  //1.Parse the query
  Parsed_Query_Ptr Parsed_Query = Parse_Query(Query);
  int num_of_relations = Get_Num_of_Relations(Parsed_Query);
  int *relations = Get_Relations(Parsed_Query);

  //new table for predicates
  Table_Ptr New_Table = Make_Table_For_Joins(Table, relations, num_of_relations);
  //execute filters first
  Execute_Filters(New_Table, Parsed_Query);


  //2.do the preparations
  Execution_Queue_Ptr Queue = Prepare_Execution_Queue(Parsed_Query, New_Table);

  //execute joins
  Intermediate_Result_Ptr Res = Execute_Joins(Queue,New_Table,Table,relations);

  //do projections
  Execute_Projections(Res, Parsed_Query, Table, Results_array, query_id);
 


  Delete_Queue(Queue);
  Delete_Parsed_Query(Parsed_Query);
  Delete_Table(New_Table);
  if(Res!=NULL)
    Delete_intermediate_Result(Res);
}

