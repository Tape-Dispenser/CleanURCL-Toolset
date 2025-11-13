
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "lib/stringutils.h"
#include "lib/stack.h"
#include <libfyaml.h>

int main() {
  static const char* yaml =
    "invoice: 34843\n"
    "date: !!str 2001-01-23\n"
    "bill-to: &id001\n"
    "  given: Chris\n"
    "  family: Dumars\n"
    "  address: \n"
    "    lines: |\n"
    "      458 Walkman Dr.\n"
    "      Suite #292\n";

  struct fy_document* fyd = NULL;
  unsigned int invoice_num;
  char given[1024];
  int count;

  fyd = fy_document_build_from_string(NULL, yaml, FY_NT);
  if (!fyd) {
    fprintf(stderr, "failed to build document\n");
    return -1;
  }

  count = fy_document_scanf(fyd, "/invoice %u /bill-to/given %1023s", &invoice_num, given);
  if (count != 2) {
    fprintf(stderr, "Failed to retrieve the two items\n");
    fy_document_destroy(fyd);
    return -1;
  }

  printf("# invoice number was %u\n", invoice_num);
  printf("# given name is %s\n", given);

  fy_document_destroy(fyd);
  return 0;
}