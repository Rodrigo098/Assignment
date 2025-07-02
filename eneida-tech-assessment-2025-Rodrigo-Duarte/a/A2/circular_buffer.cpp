#include <iostream>
#define SIZE_BUFFER 128


class CircularBuffer { 
    /*
    Esta classe é bastante simples  em termos de funcionamente
    temos um head e um tail, o head é para escrever e o tail para remover elementos
    Eu assumi que o pop fosse para remover o elemento do buffer do tail, se fosse para remover
    com base em procurar o elemento necesário teria implementado de outra forma.
    Adicionei uma classe de get_element apenas por ser aquilo que imagino que uma classe Circular buffer teria
    */
    private:
        unsigned char* buffer;
        int head;
        int tail;
    public:
         CircularBuffer() : head(0), tail(0){
            buffer = new unsigned char[SIZE_BUFFER];
        }
        
        ~CircularBuffer() {
            delete[] buffer;
        }

        void init(){
            buffer = new unsigned char[SIZE_BUFFER];
            head=0;
            tail=0;
        }

        bool push(unsigned char element){
            if(is_full()){
                return false;
            }
            buffer[head]=element;
            head= (head+1)%SIZE_BUFFER;
            return true;
        }

        unsigned char pop(){
            if(is_empty()){
                return 0; 
            }
            unsigned char res = buffer[tail];
            tail =(tail + 1) % SIZE_BUFFER;
            return res;
        }

        unsigned char get_element(int index){
            int pos=index%SIZE_BUFFER;
            if(is_empty() || pos>head){
                return 0;
            }
            return buffer[pos];
            
        }
        
        bool is_empty(){
            return head==tail;
        }

        bool is_full(){
            return ((head+1)%SIZE_BUFFER)==tail;
        }

        unsigned char * copy_buffer(){ 
            if(is_empty()) {
                return nullptr; 
            }
            unsigned char * copy = new unsigned char[SIZE_BUFFER];
            for(int i = 0; i < SIZE_BUFFER; i++) {
                copy[i] = buffer[i];
            }
            return copy;

        }
        
};

int main() {
    // Usei o copilot para me fazer uma test suite rapida para a classe
    // A classe eu criei a minha maneira mas acaba por ser generica
    CircularBuffer cb;
    
    // Test 1: Initial state should be empty
    std::cout << "=== Test 1: Initial State ===" << std::endl;
    std::cout << "Is empty: " << (cb.is_empty() ? "true" : "false") << std::endl;
    std::cout << "Is full: " << (cb.is_full() ? "true" : "false") << std::endl;
    
    // Test 2: Push some elements
    std::cout << "\n=== Test 2: Push Elements ===" << std::endl;
    for(int i = 1; i <= 5; i++) {
        bool success = cb.push(i * 10);
        std::cout << "Push " << (i * 10) << ": " << (success ? "success" : "failed") << std::endl;
    }
    
    // Test 3: Pop some elements
    std::cout << "\n=== Test 3: Pop Elements ===" << std::endl;
    for(int i = 0; i < 3; i++) {
        unsigned char value = cb.pop();
        std::cout << "Pop: " << (int)value << std::endl;
    }
    
    // Test 4: Push more elements
    std::cout << "\n=== Test 4: Push More Elements ===" << std::endl;
    for(int i = 6; i <= 10; i++) {
        bool success = cb.push(i * 10);
        std::cout << "Push " << (i * 10) << ": " << (success ? "success" : "failed") << std::endl;
    }
    
    // Test 5: Fill the buffer completely
    std::cout << "\n=== Test 5: Fill Buffer Completely ===" << std::endl;
    int count = 0;
    for(int i = 1; i <= SIZE_BUFFER; i++) {
        if(cb.push(i % 256)) {
            count++;
        } else {
            break;
        }
    }
    std::cout << "Elements pushed before full: " << count << std::endl;
    std::cout << "Is full: " << (cb.is_full() ? "true" : "false") << std::endl;
    
    // Test 6: Try to push when full
    std::cout << "\n=== Test 6: Push When Full ===" << std::endl;
    bool success = cb.push(255);
    std::cout << "Push when full: " << (success ? "success" : "failed") << std::endl;
    
    // Test 7: Empty the buffer
    std::cout << "\n=== Test 7: Empty Buffer ===" << std::endl;
    count = 0;
    while(!cb.is_empty()) {
        unsigned char value = cb.pop();
        count++;
        if(count <= 10) { // Only show first 10 values
            std::cout << "Pop " << count << ": " << (int)value << std::endl;
        }
    }
    std::cout << "Total elements popped: " << count << std::endl;
    std::cout << "Is empty: " << (cb.is_empty() ? "true" : "false") << std::endl;
    
    // Test 8: Pop from empty buffer
    std::cout << "\n=== Test 8: Pop When Empty ===" << std::endl;
    unsigned char value = cb.pop();
    std::cout << "Pop from empty buffer: " << (int)value << std::endl;
    
    return 0;
}