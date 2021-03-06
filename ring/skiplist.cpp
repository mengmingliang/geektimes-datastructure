#include<iostream>
#include<stdlib.h>
#include<time.h>

using namespace std;
#define MAX_LEVEL 16

typedef struct ListNode{
    int val;
    int max_level;
    struct ListNode * forwards[MAX_LEVEL];

    ListNode(int val){
        this->val = val;
        this->max_level = 0;
        for(int i = 0;i < MAX_LEVEL; ++i){
            this->forwards[i] = NULL;
        }
    }
}ListNode;

class SkipList{
public:
    SkipList(){
        this->head = new ListNode(-1);
        this->max_level = 0;
        this->max_level_nodes = 0;
        this->debug = false;
        srand(time(NULL));
    }

    int random_level(){
        int i = 0;
        int level = 1;

        for(int i = 1;i < MAX_LEVEL; ++i){
            if(rand()%2){
                level++;
            }
        }

        return level;
    }

    void random_level_test(){
        std::cout<<"random level"<<random_level()<<"\n\r";
        std::cout<<"random level"<<random_level()<<"\n\r";
        std::cout<<"random level"<<random_level()<<"\n\r";
        std::cout<<"random level"<<random_level()<<"\n\r";
        std::cout<<"random level"<<random_level()<<"\n\r";
    }

    bool insert(int val){
        int level = random_level();
        if(this->debug){
            cout<<"insert value:"<<val<<" at "<<level<<"level"<<endl;
        }

        ListNode * node = new ListNode(val);
        ListNode * curr = NULL;
        ListNode * next = NULL;
        ListNode * update[MAX_LEVEL];

        node->max_level = level;
        node->val = val;


        curr = this->head;
        for(int i = level-1; i >= 0; --i){
            while(curr->forwards[i]&&curr->forwards[i]->val < val){
                curr = curr->forwards[i];
            }
            update[i] = curr;
        }


        for(int i = 0;i < level; ++i){
            node->forwards[i] = update[i]->forwards[i];
            update[i]->forwards[i] = node;
        }

        if(this->max_level < level){
            this->max_level = level;
            this->max_level_nodes = 1;
        }else if(this->max_level == level){
            this->max_level_nodes++;
        }

        return true;
    }

    ListNode * find(int val){
        ListNode * node = this->head;

        for(int i = this->max_level-1;i >= 0; --i){
            while(node->forwards[i]&&node->forwards[i]->val < val){
                if(this->debug){
                    cout<<"we  search "<<node->forwards[i]->val<<endl;
                }
                node = node->forwards[i];
            }
        }

        if(node->forwards[0] && node->forwards[0]->val == val){
            return node->forwards[0];
        }else{
            return NULL;
        }
    }

    bool  remove(int val){
        ListNode *update[MAX_LEVEL];
        ListNode *curr;

        curr = this->head;
        for(int i = this->max_level-1; i >= 0; --i){
            while(curr->forwards[i]&&curr->forwards[i]->val < val){
                curr = curr->forwards[i];
            }
            update[i] = curr;
        }

        if(!curr->forwards[0]||curr->forwards[0]->val != val){
            return false;
        }

        if(curr->forwards[0]->max_level == this->max_level){
            this->max_level_nodes--;
        }

        for(int i = this->max_level; i >= 0; --i){
            if(update[i]->forwards[i] && update[i]->forwards[i]->val == val){
                update[i]->forwards[i] = update[i]->forwards[i]->forwards[i];
            }
        }


        if(curr->forwards[0]->max_level == this->max_level){
            if(this->debug){
                cout<<"we remove value:"<<curr->forwards[0]->val<<endl;
            }
            delete curr->forwards[0];
        }

        if(this->max_level_nodes == 0){
            curr = this->head;
            for(int i = this->max_level-2; i >= 0; --i){
                while(curr->forwards[i]){
                    this->max_level_nodes++;
                    curr = curr->forwards[i];
                }

                if(this->max_level_nodes > 0){
                    this->max_level = i + 1;
                    break;
                }else{
                    this->max_level = i;
                }
            }
        }

        return true;
    }

    void debug_list(){
        ListNode * node = this->head;
        for(int i = this->max_level - 1;i >= 0; --i){
            cout<<"level:"<<i+1<<" ";
            node = this->head->forwards[i];
            while(node){
                cout<<"["<<node->val<<"]->";
                node = node->forwards[i];
            }
            cout<<"\n\r";
        }
        cout<<"\n\r";
    }

    void debug_enable(){
      this->debug = true;
    }

    void debug_disable(){
      this->debug = false;
    }

private:
    ListNode  * head;
    int max_level;
    int max_level_nodes;
    bool debug;
};

int main(){
    SkipList list;
    list.debug_list();

    for(int i = 1;i <= 20; ++i){
        list.insert(rand()%100);
    }

    list.debug_list();
    //list.debug_enable();

    for(int i = 1;i <= 20; ++i){
      if(list.find(i)){
          cout<<"search "<<i<<" sucess!"<<endl;
      }
    }

    for(int i = 1;i <= 10; ++i){
        list.remove(i);
    }
    list.debug_list();

    return 0;
}
