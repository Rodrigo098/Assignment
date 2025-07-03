#include<iostream>
#include<stdio.h>
#include<map>
#include<sstream>
class MessageReceiver{

    private:
            std::map<int, std::string> messageMap; // Map of messages
            const int MESSAGES_TO_PRINT=5; // Estou a fazer assim em vez do Define para fazer uma aproach diferente, consigo adaptar me ao codigo existente
            int message_counter=0;
    public:
            void receive(uint32_t id, const std::string& text){
                if(messageMap.empty()){
                    messageMap.insert({id, text});
                    return;
                }
                std::string text_to_insert= text;
                auto range= messageMap.equal_range(id);
                if(range.first== range.second){ // Ainda não existe nenhuma key com esse valor
                    if(range.first!=messageMap.begin()){// Se fosse igual a minha key seria o menor elemento
                        auto key_before= std::prev(range.first); // Tenho de ir buscar o anterior porque o range.first envia me a primeira key maior que o valor que mandei, ver https://en.cppreference.com/w/cpp/container/map/equal_range.html
                        uint32_t id_key= key_before->first;
                        uint32_t diff= id-id_key;
                        if(diff>1){
                                std::ostringstream oss;
                                oss << "[faulty message for ID " << id_key << "]";
                                text_to_insert = oss.str();
                        }
                    }

                    if(range.second!=messageMap.end()){
                        uint32_t id_key= range.second->first; 
                        uint32_t diff= id-id_key;
                        if(diff>1){
                                std::ostringstream oss;
                                oss << "[faulty message for ID " << id_key << "]";
                                text_to_insert = oss.str();
                        }
                    }

                }
             
                auto result=messageMap.insert({id, text_to_insert});
                if(result.second){//Só uma pequena verificação se a operação foi bem sucedida
                     message_counter= (message_counter+1)%MESSAGES_TO_PRINT;
                    if(message_counter== 0){
                        print_messages(id);
                    }
                }
               
                
            }

            void print_messages(uint32_t id){
                int size_map= messageMap.size();
                if(size_map<MESSAGES_TO_PRINT) return; //

                auto iterator = messageMap.lower_bound(id);
                    
                
                int remaining_elements = std::distance(iterator, messageMap.end());
                if(remaining_elements < MESSAGES_TO_PRINT) {
                    iterator = messageMap.begin();
                }
                for(int count=0;count<MESSAGES_TO_PRINT && iterator!=messageMap.end();count++){
                    std::cout<<"Message Id "<<iterator->first<<":"<<iterator->second;
                    iterator++;
                }
            }

};
int main(){

}