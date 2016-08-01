/*
 * Copyright (c) 2016 Dell Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * THIS CODE IS PROVIDED ON AN  *AS IS* BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *  LIMITATION ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 * FOR A PARTICULAR PURPOSE, MERCHANTABLITY OR NON-INFRINGEMENT.
 *
 * See the Apache Version 2.0 License for specific language governing
 * permissions and limitations under the License.
 */

/*
 * filename: std_config_node.c
 */



#include "std_config_node.h"
#include "std_assert.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

std_config_hdl_t std_config_load(const char *filename)
{
    xmlDoc *doc = NULL;

    STD_ASSERT(filename != NULL);
    doc = xmlReadFile(filename, NULL, 0);
    return (std_config_hdl_t) doc;
}

std_config_node_t std_config_get_root(std_config_hdl_t hdl)
{
    STD_ASSERT(hdl != NULL);
    return xmlDocGetRootElement((xmlDoc *)hdl);
}

char *std_config_attr_get(std_config_node_t node, const char *attr)
{
    STD_ASSERT(node != NULL);
    return (char *)xmlGetProp((xmlNode *)node, (xmlChar *)attr);
}

std_config_node_t std_config_get_child(std_config_node_t cfg_node)
{
    xmlNode *cur_node;
    xmlNode *node = (xmlNode *)cfg_node;

    STD_ASSERT(node != NULL);

    if (node->children == NULL)
    {
        return NULL;
    }

    cur_node=node->children;

    while ((cur_node) && (cur_node->type != XML_ELEMENT_NODE))
    {
        cur_node=cur_node->next;
    }
    return (std_config_node_t)cur_node;
}

std_config_node_t std_config_next_node(std_config_node_t cfg_node)
{
    xmlNode *node = (xmlNode *)cfg_node;

    STD_ASSERT(node != NULL);
    node=node->next;

    while ((node) && (node->type != XML_ELEMENT_NODE))
    {
        node=node->next;
    }
    return (std_config_node_t)node;
}

const char *std_config_name_get(std_config_node_t node)
{
    STD_ASSERT(node != NULL);
    return (char *)(((xmlNode *)node)->name);
}

void std_config_unload(std_config_hdl_t hdl)
{
    STD_ASSERT(hdl != NULL);

    /*free the document */
    xmlFreeDoc((xmlDoc *)hdl);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}

void std_config_for_each_node(std_config_node_t node,
    void (*fn)(std_config_node_t node, void *user_data), void *user_data)
{
    std_config_node_t cur_node = NULL;

    STD_ASSERT(node != NULL);
    STD_ASSERT(fn != NULL);

    for (cur_node = std_config_get_child(node); cur_node != NULL;
        cur_node = std_config_next_node(cur_node)) {
        fn(cur_node, user_data);
    }
}
