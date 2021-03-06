﻿#pragma once
#ifndef _SQLITE_QUERY_
#define _SQLITE_QUERY_
#include <assert.h>
#include <thread>
#include <boost/shared_array.hpp>
#include <chrono>
#include <string>
#include <cstring>

#include "sqlite3.h"
struct sqlite3_stmt;
struct sqlite3;

class CSqliteDb {
 public:
  CSqliteDb(const char* dbPath);
  CSqliteDb();
  void Open(const char* dbPath);
  void Close();
  int GetLastRowID();
  ~CSqliteDb();

  operator const sqlite3*() const throw() { return _sqlite_db; }
  operator sqlite3*() throw() { return _sqlite_db; }
  sqlite3* GetDBPtr() const { return _sqlite_db; }
  bool IsTableExist(const std::string& tb_name);
  int _error_code;
  sqlite3* _sqlite_db;
};

class CSqliteOperator {
 public:
  static void SqliteExecute(CSqliteDb& sqlite_db,
                            const std::string& sql);
  static CSqliteOperator SqliteQuery(CSqliteDb& sqlite_db,
                                     const std::string& sql);

  typedef struct tagBlob {
    boost::shared_array<unsigned char> ptr;
    int length;
    tagBlob() : length(0) {}

    void SetData(const void* data, int iLen) {
      ptr = boost::shared_array<unsigned char>(new unsigned char[iLen]);
      length = iLen;
      memcpy(ptr.get(), data, iLen);
    }
  } Blob;


  struct ConstStringBlob {
    ConstStringBlob() = default;
    ConstStringBlob(const void* buffer, uint32_t len) {
      this->buffer = buffer;
      this->len = len;
    }
    void SetData(const void* buffer, int len) {
      this->buffer = buffer;
      this->len = len;
    }
    const void* buffer;
    uint32_t len;
  };

  struct StringBlob {
    StringBlob() = default;
    StringBlob(const std::string& buffer) {
      data = buffer;
    }

    std::string data;
    void SetData(const void* buffer, int iLen) {
      data.resize(iLen);
      memcpy((void*)data.data(), buffer, iLen);
    }
  };

  CSqliteOperator();
  CSqliteOperator(CSqliteDb& sqlite_db);
  ~CSqliteOperator(void);

  sqlite3_stmt* PrepareSQL(const std::string& sql);


  CSqliteOperator& operator<<(const std::string& text);
  CSqliteOperator& operator<<(unsigned char b);
  CSqliteOperator& operator<<(short s);
  CSqliteOperator& operator<<(int i);
  CSqliteOperator& operator<<(float f);
  CSqliteOperator& operator<<(double d);
  CSqliteOperator& operator<<(long long i64);
  CSqliteOperator& operator<<(Blob& blog);
  CSqliteOperator& operator<<(StringBlob& blog);
  CSqliteOperator& operator<<(ConstStringBlob& blog);
  

  CSqliteOperator& operator>>(std::string& text);
  CSqliteOperator& operator>>(unsigned char& b);
  CSqliteOperator& operator>>(short& s);
  CSqliteOperator& operator>>(int& i);
  CSqliteOperator& operator>>(float& f);
  CSqliteOperator& operator>>(double& d);
  CSqliteOperator& operator>>(long long& i64);
  CSqliteOperator& operator>>(Blob& blob);
  CSqliteOperator& operator>>(StringBlob& blob);

  bool operator()() { return Execute(); }
  void Reset();
  bool Next();
  bool Execute();
  CSqliteDb* SqliteDbPtr() { return _sqlite_db_ptr; }

  int Errcode() { return sqlite3_errcode(*_sqlite_db_ptr); }
  const char* Errmsg() { return sqlite3_errmsg(*_sqlite_db_ptr); }

 protected:
  void _Finalize();
  int _Step();

 private:
  int _cur_bind;
  sqlite3_stmt* _stmt_ptr;
  CSqliteDb* _sqlite_db_ptr;

 protected:
};

class AutoTransaction {
 public:
  AutoTransaction(sqlite3* db_ptr, bool exclusive = false);
  ~AutoTransaction();
  void Commit();

 private:
  bool m_commited;
  sqlite3* m_db_ptr;
};

class CSqliteMutex {
  /**
  SQLITE_MUTEX_FAST
  SQLITE_MUTEX_RECURSIVE
  SQLITE_MUTEX_STATIC_MASTER
  SQLITE_MUTEX_STATIC_MEM
  SQLITE_MUTEX_STATIC_MEM2
  SQLITE_MUTEX_STATIC_PRNG
  SQLITE_MUTEX_STATIC_LRU
  SQLITE_MUTEX_STATIC_LRU2
  **/
 public:
  CSqliteMutex(int mutex_type = SQLITE_MUTEX_FAST) : _mutex(0) {
    _mutex = sqlite3_mutex_alloc(mutex_type);
    assert(NULL != _mutex);
  }
  virtual ~CSqliteMutex() {
    if (_mutex)
      sqlite3_mutex_free(_mutex);
  }
  CSqliteMutex(const CSqliteMutex& mx);
  CSqliteMutex& operator=(const CSqliteMutex& mx);

  void lock() {
    if (_mutex)
      sqlite3_mutex_enter(_mutex);
  }
  void unlock() {
    if (_mutex)
      sqlite3_mutex_leave(_mutex);
  }

  bool try_lock(unsigned wait_millisec = 0) {
    auto now = std::chrono::system_clock::now();
    if (_mutex) {
      do {
        int try_res = sqlite3_mutex_try(_mutex);
        if (SQLITE_OK == try_res)
          return true;
        assert(SQLITE_BUSY == try_res);
        uint64_t dwNow = std::chrono::duration_cast<std::chrono::milliseconds>(
                             now.time_since_epoch())
                             .count();
        if (dwNow >= wait_millisec) {
          break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
      } while (true);
    }
    return false;
  }

  operator sqlite3_mutex*() const { return _mutex; }

 private:
  sqlite3_mutex* _mutex;
};
#endif
