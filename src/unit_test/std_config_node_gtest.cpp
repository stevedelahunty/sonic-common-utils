/*
 * Copyright (c) 2016 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */

/*
 * filename: std_config_node_gtest.cpp
 */


/*
 * std_config_node_gtest.cpp
 *
 */


#include "gtest/gtest.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

extern "C" {
#include "std_config_node.h"
}

/* If this is modified, ensure to update test_config_data also accordingly
 * Current test case is based on a static XML file and and corresponding arry of objects
 * that correspon to the xml data.
 * The test ensures taht every objec the parser is able to retrieve the node and data for all elements.
 */

const char test_xml_data[]=" "
"<sdi_kernel_driver instance=\"0\"> "
    "<kernel_i2c instance=\"0\" bus_name=\"SMBus SCH adapter at 0400\" > "
        "<!-- Some comment --> "
        "<tmp75 instance=\"0\" addr=\"0x4c\" low_threshold=\"10\" high_threshold=\"100\" /> "
        "<tmp75 instance=\"1\" addr=\"0x4d\" low_threshold=\"10\" high_threshold=\"100\" /> "
        "<tmp75 instance=\"2\" addr=\"0x4e\" low_threshold=\"10\" high_threshold=\"100\" /> "
    "</kernel_i2c> "
    "<kernel_i2c instance=\"1\" bus_name=\"SMBus iSMT adapter at ff782000\" > "
    "</kernel_i2c> "
"</sdi_kernel_driver> "
"";

struct test_attr_element {
    const char *name;
    const char *value;
};

#define TEST_MAX_ATTRS 4
struct test_config_node {
    const char *name;
    const char *parent;
    struct test_attr_element attrs[TEST_MAX_ATTRS];
};

/* This data corresponds the xml data pointed by test_xml_data */
const struct test_config_node test_config_data[]={
    { "sdi_kernel_driver", NULL, {
            {"instance", "0"},
            {NULL, NULL},
            {NULL, NULL},
            {NULL, NULL}}},
    { "kernel_i2c", "sdi_kernel_driver", {
            {"instance", "0"},
            {"bus_name", "SMBus SCH adapter at 0400"},
            {NULL, NULL},
            {NULL, NULL}}},
    { "tmp75", "kernel_i2c", {
            {"instance", "0"},
            {"addr", "0x4c"},
            {"low_threshold", "10"},
            {"high_threshold", "100"}}},
    { "tmp75", "kernel_i2c", {
            {"instance", "1"},
            {"addr", "0x4d"},
            {"low_threshold", "10"},
            {"high_threshold", "100"}}},
    { "tmp75", "kernel_i2c", {
            {"instance", "2"},
            {"addr", "0x4e"},
            {"low_threshold", "10"},
            {"high_threshold", "100"}}},
    { "kernel_i2c", "sdi_kernel_driver", {
            {"instance", "1"},
            {"bus_name", "SMBus iSMT adapter at ff782000"},
            {NULL, NULL},
            {NULL, NULL}}}
};


char fname[PATH_MAX];
int num_nodes=sizeof(test_config_data)/sizeof(test_config_data[0]);
int node_id=0; /* Global counter to indicate the node_id of test_config_data that
                  is being validated */

void validate_node(std_config_node_t node)
{
    const struct test_config_node *test_node_ptr=NULL;
    std_config_node_t child_node;
    int attr_id=0;
    const char *attr_name;

    test_node_ptr=&(test_config_data[node_id]);
    printf("%s:%d node_id: %d names: expected:%s found %s\n", __FUNCTION__, __LINE__, node_id,
            std_config_name_get(node), test_node_ptr->name);
    ASSERT_EQ(strcmp(std_config_name_get(node), test_node_ptr->name), 0);

    for(; (attr_id<TEST_MAX_ATTRS) && (test_node_ptr->attrs[attr_id].name != NULL);
            attr_id++)
    {
        attr_name=test_node_ptr->attrs[attr_id].name;
        printf("%s:%d attribute %s: expected:%s found %s\n", __FUNCTION__, __LINE__,
                attr_name, test_node_ptr->attrs[attr_id].value, std_config_attr_get(node, attr_name));
        ASSERT_EQ(strcmp(test_node_ptr->attrs[attr_id].value, std_config_attr_get(node, attr_name)), 0);
    }

    /* Check the child nodes */
    for (child_node=std_config_get_child(node); (child_node != NULL);
            child_node=std_config_next_node(child_node))
    {
        node_id++;
        validate_node(child_node);
    }
}

TEST(std_config_node_test, walk_elements_attributes)
{

    std_config_hdl_t file_hdl;

    file_hdl=std_config_load(fname);
    ASSERT_NE((uintptr_t)file_hdl, (uintptr_t)NULL);

    std_config_node_t node=std_config_get_root(file_hdl);

    node_id=0;
    validate_node(node);

    std_config_unload(file_hdl);
}

static int generate_xmlfile(FILE *fp)
{
  ssize_t bytes_written, bytes_to_write;
  const char *data_to_write=test_xml_data;


  bytes_to_write= sizeof(test_xml_data);
  do
  {
      bytes_written=fwrite(data_to_write,1, bytes_to_write,fp);
      if (bytes_written == EOF)
      {
          if (errno == EAGAIN)
          {
              continue;
          }
          perror("Unable to write to file: ");
          return EXIT_FAILURE;
      } else {
          bytes_to_write -= bytes_written;
          data_to_write+=bytes_written;
      }
  } while(bytes_to_write !=0);

  return 0;
}



int main(int argc, char **argv) {
  FILE *fp;
  char *tmp_fname;
  char tmp_msg[PATH_MAX];
  int rc=-1;

  strncpy(fname, "/tmp/std_config_node_text_XXXXX", sizeof(fname));
  fname[sizeof(fname)-1] = 0;

  tmp_fname = mktemp(fname);
  if ((tmp_fname == NULL) || (strlen(tmp_fname) == 0))
  {
      perror("Unable to create temporary xml file: ");
      return -1;
  }

  fp=fopen(tmp_fname,"w+");

  if (fp == NULL)
  {
      perror("Unable to create a temporary file: ");
      return -1;
  }
  rc=generate_xmlfile(fp);

  if (rc != 0)
  {
      if( fclose (fp) != 0)
      {
          snprintf(tmp_msg,PATH_MAX,"Unable to close the temporary file created:%s",tmp_fname);
          perror(tmp_msg);
      }

      rc = unlink(tmp_fname);
      if(rc != 0)
      {
          snprintf(tmp_msg,PATH_MAX,"Unable to delete the temporary file created:%s",tmp_fname);
          perror(tmp_msg);
      }
      return -1;
  }


  ::testing::InitGoogleTest(&argc, argv);
  rc=RUN_ALL_TESTS();
  if( fclose (fp) != 0)
  {
      snprintf(tmp_msg,PATH_MAX,"Unable to close the temporary file created:%s",tmp_fname);
      perror(tmp_msg);
  }
  rc = unlink(tmp_fname);
  if(rc != 0)
  {
        snprintf(tmp_msg,PATH_MAX,"Unable to delete the temporary file created:%s",tmp_fname);
      perror(tmp_msg);
  }
  return rc;
}

