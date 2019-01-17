#include <stdio.h>
#include "rb_tree.h"

struct rb_root tree_map = RB_ROOT;

typedef struct key_value{
    struct rb_node node;
    int val;
    int idx;
}key_value;

/*insert binding*/
int tree_binding_insert(struct rb_root *root,key_value *p)
{
    struct rb_node **new = &(root->rb_node);
  	struct rb_node *parent = NULL;
  	key_value *this;
  	int result;

    while(*new){
        this = container_of(node,key_value,node);
        parent = *new;

        if(this->val > p->val){
            new = &((*new)->rb_right);
        }else if(this->val < p->val){
            new = &((*new)->rb_left);
        }else{
            this->idx = p->idx;
            return 1;
        }
    }

    /* Add new node and rebalance tree. */
  	rb_link_node(&p->node, parent, new);
  	rb_insert_color(&p->node, root);

    return 0;
}

/*remove key from rbtree*/
int tree_binding_del(struct rb_root *root, int val)
{
	key_value * entry = tree_binding_search(root, mac_addr);
	if(entry){
		rb_erase(&entry->node, root);
		free(entry);
	}else{
		return -1;
	}	
	return 0;		
}

/*search key*/
key_value * tree_binding_search(struct rb_root *root,int val)
{
    struct rb_node *node = root->rb_node;
    key_value * data;

    while(node){
        data = container_of(node,key_value,node);
        if(data->val == val){
            return data;
        }else if(data->val > val){
            node = node->right;
        }else{
            node = node->left;
        }
    }

    return NULL;
}

int tree_binding_free(struct rb_root *root)
{
	struct rb_node *anode;
	key_value * entry;
	
	anode = rb_first(root);
	
	while(anode){
		entry = NULL;
		entry = container_of(anode, key_value, node);
		anode = rb_next(anode);
		rb_erase(&entry->node, root);
		if(entry!=NULL)
			free(entry);
	}

	return 0;
}

key_value * tree_binding_add(struct rb_root *root, int val,int idx)
{
	key_value * entry = NULL;	
	key_value * data;
	
	data = tree_binding_search(root, val);
	
	if(data){
		data->idx = idx;
	}	
	else{
		entry = (key_value*)malloc(sizeof(key_value));
		if(!entry)
			return NULL;

	
		entry->val = val;
		entry->idx = idx;
		tree_binding_insert(root, entry);
	}
	
	if(entry!=NULL)
		return entry;
	else
		return data;
}

key_value * lower_bound(struct rb_root *root,int val)
{
    struct rb_node *node = root->rb_node;
    key_value * data;

    while(node){
        data = container_of(node,key_value,node);
        if(data->val == val){
            return data;
        }else if(data->val > val){
            node = node->left;
        }else{
            if(!node->right){
                return data;
            }else{
                node = node->right;
            }
        }
    }

    return NULL;
}

key_value * upper_bound(struct rb_root *root,int val)
{
    struct rb_node *node = root->rb_node;
    key_value * data;

    while(node){
        data = container_of(node,key_value,node);
        if(data->val == val){
            return data;
        }else if(data->val < val){
            node = node->right;
        }else{
            if(!node->left){
                return data;
            }else{
                node = node->left;
            }
        }
    }

    return NULL;
}

int main()
{
    int i = 0;
	key_value * entry;
    srand((unsigned)time(NULL))
    
	printf("Creating data...\n");
	
	for(i = 0; i<65500 ; i++)
	{
		tree_binding_add(tree_map,rand()%65535,i);
	}
	
	printf("Searching...\n");
    for(i = 0;i < 65535; ++i){
        entry = tree_binding_search(tree_map, i);
        if(entry){
            printf("%d:%d\n\r",entry->val,entry->idx);
        }
    }
	
	return 0;
}


