#ifndef FIFO_UDF_LEN_H
#define FIFO_UDF_LEN_H

typedef unsigned int uint;

#define INITIALIZE_FIFO 0
#define MAX_OF_UINT     4294967295

template <typename T>
class FIFO{

public:
    FIFO()
        : _count(0)
        , _fifo_len(1)
        , _initialized(false)
        , _accumulation(0)
    {}

    ~FIFO(){
        if(_initialized){
            delete []_fifo;
        }
    }

    void init(uint len);
    void destroy();
    void to_fifo(T data);
    void update();
    uint get_count() { return _count; }
    uint get_length(){ return _fifo_len; }

    T average;
    T min;
    T max;

private:
    void _min();
    void _max();
    void _average();

    uint _count;
    uint _fifo_len;
    bool _initialized;
    T    _data;
    T    _accumulation;
    T*   _fifo;
};

template <typename T>
void FIFO<T>::init(uint len){
    if(len > 0){
        if(len > MAX_OF_UINT){
            _fifo_len = MAX_OF_UINT;
        }else{
            _fifo_len = len;
        }
    }
    if(_initialized){
        delete []_fifo;
    }
    _fifo         = new T[_fifo_len];
    _accumulation = 0;
    _count        = 0;
    _initialized  = true;
    memset(_fifo, INITIALIZE_FIFO, sizeof(T)*_fifo_len);
}

template <typename T>
void FIFO<T>::destroy(){
    if(_initialized){
        delete []_fifo;
    }
}

template <typename T>
void FIFO<T>::to_fifo(T data){
    _data = data;
    update();
}

template <typename T>
void FIFO<T>::update(){
    if(_count == MAX_OF_UINT){
        _count = 0;
    }
    _count++;    
    if(_count < _fifo_len){
        *(_fifo + (_count -1)) = _data;
    }else{
        *(_fifo + (_count % _fifo_len)) = _data;
    }
    _min();
    _max();
    _average();
}

template <typename T>
void FIFO<T>::_min(){
    T temp_min = *(_fifo);
    for(uint i=1; i<_fifo_len; i++){
        if(*(_fifo+i)<temp_min && *(_fifo+i)!=INITIALIZE_FIFO){
            temp_min = *(_fifo+i);
        }
    }
    min = temp_min;
}

template <typename T>
void FIFO<T>::_max(){
    T temp_max = *(_fifo);
    for(uint i=1; i<_fifo_len; i++){
        if(*(_fifo+i)>temp_max && *(_fifo+i)!=INITIALIZE_FIFO){
            temp_max = *(_fifo+i);
        }
    }
    max = temp_max;
}

template <typename T>
void FIFO<T>::_average(){
    if(_count > 0){
        if(_count < _fifo_len){
            _accumulation += _data;
            average        = _accumulation/_count;
        }else{
            _accumulation  = 0;
            for(uint i=0; i<_fifo_len; i++){
                _accumulation += *(_fifo + i);
            }
            average = _accumulation/_fifo_len;
        }
    }
}

#endif
