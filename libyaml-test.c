#include <stdio.h>
#include <yaml.h>
yaml_document_t document;
void dump(yaml_node_t*node){
  switch(node->type){
    break;case YAML_NO_NODE:
    printf("n/a");
    break;case YAML_SCALAR_NODE:
    switch(node->data.scalar.style){
      break;case YAML_ANY_SCALAR_STYLE:
      printf("?%s?", node->data.scalar.value);
      break;case YAML_PLAIN_SCALAR_STYLE:
      printf("%s", node->data.scalar.value);
      break;case YAML_SINGLE_QUOTED_SCALAR_STYLE:
      printf("'%s'", node->data.scalar.value);
      break;case YAML_DOUBLE_QUOTED_SCALAR_STYLE:
      printf("\"%s\"", node->data.scalar.value);
      break;case YAML_LITERAL_SCALAR_STYLE:
      printf("L%sL", node->data.scalar.value);
      break;case YAML_FOLDED_SCALAR_STYLE:
      printf("folded[%s]", node->data.scalar.value);
    }
    break;case YAML_SEQUENCE_NODE:
    printf("[ ");
    yaml_node_item_t*ie    = node->data.sequence.items.top;
    for(yaml_node_item_t*i = node->data.sequence.items.start;i<ie;i++)
      dump(yaml_document_get_node(&document, *i)),
	printf(", ");
    printf("]");
    break;case YAML_MAPPING_NODE:
    printf("{ ");
    yaml_node_pair_t*pe = node->data.mapping.pairs.top;
    for(yaml_node_pair_t*p = node->data.mapping.pairs.start;p<pe;p++)
      dump(yaml_document_get_node(&document, p->key)),
	printf(": "),
	dump(yaml_document_get_node(&document, p->value)),
	printf(", ");
    printf("}");
  }
}
int main(int argc, char**argv){
  yaml_parser_t parser;
  yaml_node_t*node;
  FILE*fh = fopen(argv[1],"r");
  /* Initialize parser */
  if(!yaml_parser_initialize(&parser))
    fputs("Failed to initialize parser!\n", stderr);
  if(fh == NULL)
    fputs("Failed to open file!\n", stderr);

  /* Set input file */
  yaml_parser_set_input_file(&parser, fh);

  if(!yaml_parser_load(&parser, &document))
    fputs("Failed to load parser!\n", stderr);

  node = yaml_document_get_root_node(&document);
  
  printf("root %p\n", node);
  dump(node);
  
}
