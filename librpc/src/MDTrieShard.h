/**
 * Autogenerated by Thrift Compiler (0.15.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef MDTrieShard_H
#define MDTrieShard_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "rpc_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class MDTrieShardIf {
 public:
  virtual ~MDTrieShardIf() {}
  virtual bool ping(const int32_t dataset_idx) = 0;
  virtual int32_t add(const int32_t num1, const int32_t num2) = 0;
  virtual int32_t insert(const std::vector<int32_t> & point) = 0;
  virtual bool check(const std::vector<int32_t> & point) = 0;
  virtual void range_search(std::vector<std::vector<int32_t> > & _return, const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range) = 0;
  virtual void primary_key_lookup(std::vector<int32_t> & _return, const int32_t primary_key) = 0;
  virtual int32_t get_size() = 0;
  virtual void clear_trie() = 0;
};

class MDTrieShardIfFactory {
 public:
  typedef MDTrieShardIf Handler;

  virtual ~MDTrieShardIfFactory() {}

  virtual MDTrieShardIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(MDTrieShardIf* /* handler */) = 0;
  };

class MDTrieShardIfSingletonFactory : virtual public MDTrieShardIfFactory {
 public:
  MDTrieShardIfSingletonFactory(const ::std::shared_ptr<MDTrieShardIf>& iface) : iface_(iface) {}
  virtual ~MDTrieShardIfSingletonFactory() {}

  virtual MDTrieShardIf* getHandler(const ::apache::thrift::TConnectionInfo&) override {
    return iface_.get();
  }
  virtual void releaseHandler(MDTrieShardIf* /* handler */) override {}

 protected:
  ::std::shared_ptr<MDTrieShardIf> iface_;
};

class MDTrieShardNull : virtual public MDTrieShardIf {
 public:
  virtual ~MDTrieShardNull() {}
  bool ping(const int32_t /* dataset_idx */) override {
    bool _return = false;
    return _return;
  }
  int32_t add(const int32_t /* num1 */, const int32_t /* num2 */) override {
    int32_t _return = 0;
    return _return;
  }
  int32_t insert(const std::vector<int32_t> & /* point */) override {
    int32_t _return = 0;
    return _return;
  }
  bool check(const std::vector<int32_t> & /* point */) override {
    bool _return = false;
    return _return;
  }
  void range_search(std::vector<std::vector<int32_t> > & /* _return */, const std::vector<int32_t> & /* start_range */, const std::vector<int32_t> & /* end_range */) override {
    return;
  }
  void primary_key_lookup(std::vector<int32_t> & /* _return */, const int32_t /* primary_key */) override {
    return;
  }
  int32_t get_size() override {
    int32_t _return = 0;
    return _return;
  }
  void clear_trie() override {
    return;
  }
};

typedef struct _MDTrieShard_ping_args__isset {
  _MDTrieShard_ping_args__isset() : dataset_idx(false) {}
  bool dataset_idx :1;
} _MDTrieShard_ping_args__isset;

class MDTrieShard_ping_args {
 public:

  MDTrieShard_ping_args(const MDTrieShard_ping_args&) noexcept;
  MDTrieShard_ping_args& operator=(const MDTrieShard_ping_args&) noexcept;
  MDTrieShard_ping_args() noexcept
                        : dataset_idx(0) {
  }

  virtual ~MDTrieShard_ping_args() noexcept;
  int32_t dataset_idx;

  _MDTrieShard_ping_args__isset __isset;

  void __set_dataset_idx(const int32_t val);

  bool operator == (const MDTrieShard_ping_args & rhs) const
  {
    if (!(dataset_idx == rhs.dataset_idx))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_ping_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_ping_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_ping_pargs {
 public:


  virtual ~MDTrieShard_ping_pargs() noexcept;
  const int32_t* dataset_idx;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_ping_result__isset {
  _MDTrieShard_ping_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_ping_result__isset;

class MDTrieShard_ping_result {
 public:

  MDTrieShard_ping_result(const MDTrieShard_ping_result&) noexcept;
  MDTrieShard_ping_result& operator=(const MDTrieShard_ping_result&) noexcept;
  MDTrieShard_ping_result() noexcept
                          : success(0) {
  }

  virtual ~MDTrieShard_ping_result() noexcept;
  bool success;

  _MDTrieShard_ping_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const MDTrieShard_ping_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_ping_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_ping_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_ping_presult__isset {
  _MDTrieShard_ping_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_ping_presult__isset;

class MDTrieShard_ping_presult {
 public:


  virtual ~MDTrieShard_ping_presult() noexcept;
  bool* success;

  _MDTrieShard_ping_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

typedef struct _MDTrieShard_add_args__isset {
  _MDTrieShard_add_args__isset() : num1(false), num2(false) {}
  bool num1 :1;
  bool num2 :1;
} _MDTrieShard_add_args__isset;

class MDTrieShard_add_args {
 public:

  MDTrieShard_add_args(const MDTrieShard_add_args&) noexcept;
  MDTrieShard_add_args& operator=(const MDTrieShard_add_args&) noexcept;
  MDTrieShard_add_args() noexcept
                       : num1(0),
                         num2(0) {
  }

  virtual ~MDTrieShard_add_args() noexcept;
  int32_t num1;
  int32_t num2;

  _MDTrieShard_add_args__isset __isset;

  void __set_num1(const int32_t val);

  void __set_num2(const int32_t val);

  bool operator == (const MDTrieShard_add_args & rhs) const
  {
    if (!(num1 == rhs.num1))
      return false;
    if (!(num2 == rhs.num2))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_add_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_add_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_add_pargs {
 public:


  virtual ~MDTrieShard_add_pargs() noexcept;
  const int32_t* num1;
  const int32_t* num2;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_add_result__isset {
  _MDTrieShard_add_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_add_result__isset;

class MDTrieShard_add_result {
 public:

  MDTrieShard_add_result(const MDTrieShard_add_result&) noexcept;
  MDTrieShard_add_result& operator=(const MDTrieShard_add_result&) noexcept;
  MDTrieShard_add_result() noexcept
                         : success(0) {
  }

  virtual ~MDTrieShard_add_result() noexcept;
  int32_t success;

  _MDTrieShard_add_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const MDTrieShard_add_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_add_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_add_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_add_presult__isset {
  _MDTrieShard_add_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_add_presult__isset;

class MDTrieShard_add_presult {
 public:


  virtual ~MDTrieShard_add_presult() noexcept;
  int32_t* success;

  _MDTrieShard_add_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

typedef struct _MDTrieShard_insert_args__isset {
  _MDTrieShard_insert_args__isset() : point(false) {}
  bool point :1;
} _MDTrieShard_insert_args__isset;

class MDTrieShard_insert_args {
 public:

  MDTrieShard_insert_args(const MDTrieShard_insert_args&);
  MDTrieShard_insert_args& operator=(const MDTrieShard_insert_args&);
  MDTrieShard_insert_args() noexcept {
  }

  virtual ~MDTrieShard_insert_args() noexcept;
  std::vector<int32_t>  point;

  _MDTrieShard_insert_args__isset __isset;

  void __set_point(const std::vector<int32_t> & val);

  bool operator == (const MDTrieShard_insert_args & rhs) const
  {
    if (!(point == rhs.point))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_insert_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_insert_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_insert_pargs {
 public:


  virtual ~MDTrieShard_insert_pargs() noexcept;
  const std::vector<int32_t> * point;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_insert_result__isset {
  _MDTrieShard_insert_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_insert_result__isset;

class MDTrieShard_insert_result {
 public:

  MDTrieShard_insert_result(const MDTrieShard_insert_result&) noexcept;
  MDTrieShard_insert_result& operator=(const MDTrieShard_insert_result&) noexcept;
  MDTrieShard_insert_result() noexcept
                            : success(0) {
  }

  virtual ~MDTrieShard_insert_result() noexcept;
  int32_t success;

  _MDTrieShard_insert_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const MDTrieShard_insert_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_insert_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_insert_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_insert_presult__isset {
  _MDTrieShard_insert_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_insert_presult__isset;

class MDTrieShard_insert_presult {
 public:


  virtual ~MDTrieShard_insert_presult() noexcept;
  int32_t* success;

  _MDTrieShard_insert_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

typedef struct _MDTrieShard_check_args__isset {
  _MDTrieShard_check_args__isset() : point(false) {}
  bool point :1;
} _MDTrieShard_check_args__isset;

class MDTrieShard_check_args {
 public:

  MDTrieShard_check_args(const MDTrieShard_check_args&);
  MDTrieShard_check_args& operator=(const MDTrieShard_check_args&);
  MDTrieShard_check_args() noexcept {
  }

  virtual ~MDTrieShard_check_args() noexcept;
  std::vector<int32_t>  point;

  _MDTrieShard_check_args__isset __isset;

  void __set_point(const std::vector<int32_t> & val);

  bool operator == (const MDTrieShard_check_args & rhs) const
  {
    if (!(point == rhs.point))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_check_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_check_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_check_pargs {
 public:


  virtual ~MDTrieShard_check_pargs() noexcept;
  const std::vector<int32_t> * point;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_check_result__isset {
  _MDTrieShard_check_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_check_result__isset;

class MDTrieShard_check_result {
 public:

  MDTrieShard_check_result(const MDTrieShard_check_result&) noexcept;
  MDTrieShard_check_result& operator=(const MDTrieShard_check_result&) noexcept;
  MDTrieShard_check_result() noexcept
                           : success(0) {
  }

  virtual ~MDTrieShard_check_result() noexcept;
  bool success;

  _MDTrieShard_check_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const MDTrieShard_check_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_check_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_check_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_check_presult__isset {
  _MDTrieShard_check_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_check_presult__isset;

class MDTrieShard_check_presult {
 public:


  virtual ~MDTrieShard_check_presult() noexcept;
  bool* success;

  _MDTrieShard_check_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

typedef struct _MDTrieShard_range_search_args__isset {
  _MDTrieShard_range_search_args__isset() : start_range(false), end_range(false) {}
  bool start_range :1;
  bool end_range :1;
} _MDTrieShard_range_search_args__isset;

class MDTrieShard_range_search_args {
 public:

  MDTrieShard_range_search_args(const MDTrieShard_range_search_args&);
  MDTrieShard_range_search_args& operator=(const MDTrieShard_range_search_args&);
  MDTrieShard_range_search_args() noexcept {
  }

  virtual ~MDTrieShard_range_search_args() noexcept;
  std::vector<int32_t>  start_range;
  std::vector<int32_t>  end_range;

  _MDTrieShard_range_search_args__isset __isset;

  void __set_start_range(const std::vector<int32_t> & val);

  void __set_end_range(const std::vector<int32_t> & val);

  bool operator == (const MDTrieShard_range_search_args & rhs) const
  {
    if (!(start_range == rhs.start_range))
      return false;
    if (!(end_range == rhs.end_range))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_range_search_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_range_search_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_range_search_pargs {
 public:


  virtual ~MDTrieShard_range_search_pargs() noexcept;
  const std::vector<int32_t> * start_range;
  const std::vector<int32_t> * end_range;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_range_search_result__isset {
  _MDTrieShard_range_search_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_range_search_result__isset;

class MDTrieShard_range_search_result {
 public:

  MDTrieShard_range_search_result(const MDTrieShard_range_search_result&);
  MDTrieShard_range_search_result& operator=(const MDTrieShard_range_search_result&);
  MDTrieShard_range_search_result() noexcept {
  }

  virtual ~MDTrieShard_range_search_result() noexcept;
  std::vector<std::vector<int32_t> >  success;

  _MDTrieShard_range_search_result__isset __isset;

  void __set_success(const std::vector<std::vector<int32_t> > & val);

  bool operator == (const MDTrieShard_range_search_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_range_search_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_range_search_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_range_search_presult__isset {
  _MDTrieShard_range_search_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_range_search_presult__isset;

class MDTrieShard_range_search_presult {
 public:


  virtual ~MDTrieShard_range_search_presult() noexcept;
  std::vector<std::vector<int32_t> > * success;

  _MDTrieShard_range_search_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

typedef struct _MDTrieShard_primary_key_lookup_args__isset {
  _MDTrieShard_primary_key_lookup_args__isset() : primary_key(false) {}
  bool primary_key :1;
} _MDTrieShard_primary_key_lookup_args__isset;

class MDTrieShard_primary_key_lookup_args {
 public:

  MDTrieShard_primary_key_lookup_args(const MDTrieShard_primary_key_lookup_args&) noexcept;
  MDTrieShard_primary_key_lookup_args& operator=(const MDTrieShard_primary_key_lookup_args&) noexcept;
  MDTrieShard_primary_key_lookup_args() noexcept
                                      : primary_key(0) {
  }

  virtual ~MDTrieShard_primary_key_lookup_args() noexcept;
  int32_t primary_key;

  _MDTrieShard_primary_key_lookup_args__isset __isset;

  void __set_primary_key(const int32_t val);

  bool operator == (const MDTrieShard_primary_key_lookup_args & rhs) const
  {
    if (!(primary_key == rhs.primary_key))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_primary_key_lookup_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_primary_key_lookup_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_primary_key_lookup_pargs {
 public:


  virtual ~MDTrieShard_primary_key_lookup_pargs() noexcept;
  const int32_t* primary_key;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_primary_key_lookup_result__isset {
  _MDTrieShard_primary_key_lookup_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_primary_key_lookup_result__isset;

class MDTrieShard_primary_key_lookup_result {
 public:

  MDTrieShard_primary_key_lookup_result(const MDTrieShard_primary_key_lookup_result&);
  MDTrieShard_primary_key_lookup_result& operator=(const MDTrieShard_primary_key_lookup_result&);
  MDTrieShard_primary_key_lookup_result() noexcept {
  }

  virtual ~MDTrieShard_primary_key_lookup_result() noexcept;
  std::vector<int32_t>  success;

  _MDTrieShard_primary_key_lookup_result__isset __isset;

  void __set_success(const std::vector<int32_t> & val);

  bool operator == (const MDTrieShard_primary_key_lookup_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_primary_key_lookup_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_primary_key_lookup_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_primary_key_lookup_presult__isset {
  _MDTrieShard_primary_key_lookup_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_primary_key_lookup_presult__isset;

class MDTrieShard_primary_key_lookup_presult {
 public:


  virtual ~MDTrieShard_primary_key_lookup_presult() noexcept;
  std::vector<int32_t> * success;

  _MDTrieShard_primary_key_lookup_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};


class MDTrieShard_get_size_args {
 public:

  MDTrieShard_get_size_args(const MDTrieShard_get_size_args&) noexcept;
  MDTrieShard_get_size_args& operator=(const MDTrieShard_get_size_args&) noexcept;
  MDTrieShard_get_size_args() noexcept {
  }

  virtual ~MDTrieShard_get_size_args() noexcept;

  bool operator == (const MDTrieShard_get_size_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MDTrieShard_get_size_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_get_size_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_get_size_pargs {
 public:


  virtual ~MDTrieShard_get_size_pargs() noexcept;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_get_size_result__isset {
  _MDTrieShard_get_size_result__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_get_size_result__isset;

class MDTrieShard_get_size_result {
 public:

  MDTrieShard_get_size_result(const MDTrieShard_get_size_result&) noexcept;
  MDTrieShard_get_size_result& operator=(const MDTrieShard_get_size_result&) noexcept;
  MDTrieShard_get_size_result() noexcept
                              : success(0) {
  }

  virtual ~MDTrieShard_get_size_result() noexcept;
  int32_t success;

  _MDTrieShard_get_size_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const MDTrieShard_get_size_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const MDTrieShard_get_size_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_get_size_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};

typedef struct _MDTrieShard_get_size_presult__isset {
  _MDTrieShard_get_size_presult__isset() : success(false) {}
  bool success :1;
} _MDTrieShard_get_size_presult__isset;

class MDTrieShard_get_size_presult {
 public:


  virtual ~MDTrieShard_get_size_presult() noexcept;
  int32_t* success;

  _MDTrieShard_get_size_presult__isset __isset;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};


class MDTrieShard_clear_trie_args {
 public:

  MDTrieShard_clear_trie_args(const MDTrieShard_clear_trie_args&) noexcept;
  MDTrieShard_clear_trie_args& operator=(const MDTrieShard_clear_trie_args&) noexcept;
  MDTrieShard_clear_trie_args() noexcept {
  }

  virtual ~MDTrieShard_clear_trie_args() noexcept;

  bool operator == (const MDTrieShard_clear_trie_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MDTrieShard_clear_trie_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_clear_trie_args & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_clear_trie_pargs {
 public:


  virtual ~MDTrieShard_clear_trie_pargs() noexcept;

  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_clear_trie_result {
 public:

  MDTrieShard_clear_trie_result(const MDTrieShard_clear_trie_result&) noexcept;
  MDTrieShard_clear_trie_result& operator=(const MDTrieShard_clear_trie_result&) noexcept;
  MDTrieShard_clear_trie_result() noexcept {
  }

  virtual ~MDTrieShard_clear_trie_result() noexcept;

  bool operator == (const MDTrieShard_clear_trie_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const MDTrieShard_clear_trie_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MDTrieShard_clear_trie_result & ) const;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);
  template <class Protocol_>
  uint32_t write(Protocol_* oprot) const;

};


class MDTrieShard_clear_trie_presult {
 public:


  virtual ~MDTrieShard_clear_trie_presult() noexcept;

  template <class Protocol_>
  uint32_t read(Protocol_* iprot);

};

template <class Protocol_>
class MDTrieShardClientT : virtual public MDTrieShardIf {
 public:
  MDTrieShardClientT(std::shared_ptr< Protocol_> prot) {
    setProtocolT(prot);
  }
  MDTrieShardClientT(std::shared_ptr< Protocol_> iprot, std::shared_ptr< Protocol_> oprot) {
    setProtocolT(iprot,oprot);
  }
 private:
  void setProtocolT(std::shared_ptr< Protocol_> prot) {
  setProtocolT(prot,prot);
  }
  void setProtocolT(std::shared_ptr< Protocol_> iprot, std::shared_ptr< Protocol_> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return this->piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return this->poprot_;
  }
  bool ping(const int32_t dataset_idx) override;
  void send_ping(const int32_t dataset_idx);
  bool recv_ping();
  int32_t add(const int32_t num1, const int32_t num2) override;
  void send_add(const int32_t num1, const int32_t num2);
  int32_t recv_add();
  int32_t insert(const std::vector<int32_t> & point) override;
  void send_insert(const std::vector<int32_t> & point);
  int32_t recv_insert();
  bool check(const std::vector<int32_t> & point) override;
  void send_check(const std::vector<int32_t> & point);
  bool recv_check();
  void range_search(std::vector<std::vector<int32_t> > & _return, const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range) override;
  void send_range_search(const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range);
  void recv_range_search(std::vector<std::vector<int32_t> > & _return);
  void primary_key_lookup(std::vector<int32_t> & _return, const int32_t primary_key) override;
  void send_primary_key_lookup(const int32_t primary_key);
  void recv_primary_key_lookup(std::vector<int32_t> & _return);
  int32_t get_size() override;
  void send_get_size();
  int32_t recv_get_size();
  void clear_trie() override;
  void send_clear_trie();
  void recv_clear_trie();
 protected:
  std::shared_ptr< Protocol_> piprot_;
  std::shared_ptr< Protocol_> poprot_;
  Protocol_* iprot_;
  Protocol_* oprot_;
};

typedef MDTrieShardClientT< ::apache::thrift::protocol::TProtocol> MDTrieShardClient;

template <class Protocol_>
class MDTrieShardProcessorT : public ::apache::thrift::TDispatchProcessorT<Protocol_> {
 protected:
  ::std::shared_ptr<MDTrieShardIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext) override;
  virtual bool dispatchCallTemplated(Protocol_* iprot, Protocol_* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (MDTrieShardProcessorT::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef void (MDTrieShardProcessorT::*SpecializedProcessFunction)(int32_t, Protocol_*, Protocol_*, void*);
  struct ProcessFunctions {
    ProcessFunction generic;
    SpecializedProcessFunction specialized;
    ProcessFunctions(ProcessFunction g, SpecializedProcessFunction s) :
      generic(g),
      specialized(s) {}
    ProcessFunctions() : generic(nullptr), specialized(nullptr) {}
  };
  typedef std::map<std::string, ProcessFunctions> ProcessMap;
  ProcessMap processMap_;
  void process_ping(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_ping(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_add(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_add(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_insert(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_insert(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_check(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_check(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_range_search(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_range_search(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_primary_key_lookup(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_primary_key_lookup(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_get_size(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_size(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
  void process_clear_trie(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_clear_trie(int32_t seqid, Protocol_* iprot, Protocol_* oprot, void* callContext);
 public:
  MDTrieShardProcessorT(::std::shared_ptr<MDTrieShardIf> iface) :
    iface_(iface) {
    processMap_["ping"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_ping,
      &MDTrieShardProcessorT::process_ping);
    processMap_["add"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_add,
      &MDTrieShardProcessorT::process_add);
    processMap_["insert"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_insert,
      &MDTrieShardProcessorT::process_insert);
    processMap_["check"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_check,
      &MDTrieShardProcessorT::process_check);
    processMap_["range_search"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_range_search,
      &MDTrieShardProcessorT::process_range_search);
    processMap_["primary_key_lookup"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_primary_key_lookup,
      &MDTrieShardProcessorT::process_primary_key_lookup);
    processMap_["get_size"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_get_size,
      &MDTrieShardProcessorT::process_get_size);
    processMap_["clear_trie"] = ProcessFunctions(
      &MDTrieShardProcessorT::process_clear_trie,
      &MDTrieShardProcessorT::process_clear_trie);
  }

  virtual ~MDTrieShardProcessorT() {}
};

typedef MDTrieShardProcessorT< ::apache::thrift::protocol::TDummyProtocol > MDTrieShardProcessor;

template <class Protocol_>
class MDTrieShardProcessorFactoryT : public ::apache::thrift::TProcessorFactory {
 public:
  MDTrieShardProcessorFactoryT(const ::std::shared_ptr< MDTrieShardIfFactory >& handlerFactory) noexcept :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo) override;

 protected:
  ::std::shared_ptr< MDTrieShardIfFactory > handlerFactory_;
};

typedef MDTrieShardProcessorFactoryT< ::apache::thrift::protocol::TDummyProtocol > MDTrieShardProcessorFactory;

class MDTrieShardMultiface : virtual public MDTrieShardIf {
 public:
  MDTrieShardMultiface(std::vector<std::shared_ptr<MDTrieShardIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~MDTrieShardMultiface() {}
 protected:
  std::vector<std::shared_ptr<MDTrieShardIf> > ifaces_;
  MDTrieShardMultiface() {}
  void add(::std::shared_ptr<MDTrieShardIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  bool ping(const int32_t dataset_idx) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->ping(dataset_idx);
    }
    return ifaces_[i]->ping(dataset_idx);
  }

  int32_t add(const int32_t num1, const int32_t num2) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->add(num1, num2);
    }
    return ifaces_[i]->add(num1, num2);
  }

  int32_t insert(const std::vector<int32_t> & point) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->insert(point);
    }
    return ifaces_[i]->insert(point);
  }

  bool check(const std::vector<int32_t> & point) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->check(point);
    }
    return ifaces_[i]->check(point);
  }

  void range_search(std::vector<std::vector<int32_t> > & _return, const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->range_search(_return, start_range, end_range);
    }
    ifaces_[i]->range_search(_return, start_range, end_range);
    return;
  }

  void primary_key_lookup(std::vector<int32_t> & _return, const int32_t primary_key) override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->primary_key_lookup(_return, primary_key);
    }
    ifaces_[i]->primary_key_lookup(_return, primary_key);
    return;
  }

  int32_t get_size() override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_size();
    }
    return ifaces_[i]->get_size();
  }

  void clear_trie() override {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->clear_trie();
    }
    ifaces_[i]->clear_trie();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
template <class Protocol_>
class MDTrieShardConcurrentClientT : virtual public MDTrieShardIf {
 public:
  MDTrieShardConcurrentClientT(std::shared_ptr< Protocol_> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocolT(prot);
  }
  MDTrieShardConcurrentClientT(std::shared_ptr< Protocol_> iprot, std::shared_ptr< Protocol_> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocolT(iprot,oprot);
  }
 private:
  void setProtocolT(std::shared_ptr< Protocol_> prot) {
  setProtocolT(prot,prot);
  }
  void setProtocolT(std::shared_ptr< Protocol_> iprot, std::shared_ptr< Protocol_> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return this->piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return this->poprot_;
  }
  bool ping(const int32_t dataset_idx) override;
  int32_t send_ping(const int32_t dataset_idx);
  bool recv_ping(const int32_t seqid);
  int32_t add(const int32_t num1, const int32_t num2) override;
  int32_t send_add(const int32_t num1, const int32_t num2);
  int32_t recv_add(const int32_t seqid);
  int32_t insert(const std::vector<int32_t> & point) override;
  int32_t send_insert(const std::vector<int32_t> & point);
  int32_t recv_insert(const int32_t seqid);
  bool check(const std::vector<int32_t> & point) override;
  int32_t send_check(const std::vector<int32_t> & point);
  bool recv_check(const int32_t seqid);
  void range_search(std::vector<std::vector<int32_t> > & _return, const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range) override;
  int32_t send_range_search(const std::vector<int32_t> & start_range, const std::vector<int32_t> & end_range);
  void recv_range_search(std::vector<std::vector<int32_t> > & _return, const int32_t seqid);
  void primary_key_lookup(std::vector<int32_t> & _return, const int32_t primary_key) override;
  int32_t send_primary_key_lookup(const int32_t primary_key);
  void recv_primary_key_lookup(std::vector<int32_t> & _return, const int32_t seqid);
  int32_t get_size() override;
  int32_t send_get_size();
  int32_t recv_get_size(const int32_t seqid);
  void clear_trie() override;
  int32_t send_clear_trie();
  void recv_clear_trie(const int32_t seqid);
 protected:
  std::shared_ptr< Protocol_> piprot_;
  std::shared_ptr< Protocol_> poprot_;
  Protocol_* iprot_;
  Protocol_* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

typedef MDTrieShardConcurrentClientT< ::apache::thrift::protocol::TProtocol> MDTrieShardConcurrentClient;

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#include "MDTrieShard.tcc"
#include "rpc_types.tcc"

#endif
