//#include "stdafx.h"
#include "./SqliteOperator.h"

void CSqliteOperator::SqliteExecute(CSqliteDb& sqlite_db,
                                    const std::string& sql) {
  CSqliteOperator sqliteOperator(sqlite_db);
  sqliteOperator.PrepareSQL(sql);
  sqliteOperator.Execute();
}

CSqliteOperator CSqliteOperator::SqliteQuery(CSqliteDb& sqlite_db,
                                             const std::string& sql) {
  CSqliteOperator sqliteOperator(sqlite_db);
  sqliteOperator.PrepareSQL(sql);
  return sqliteOperator;
}

//////////////////////////////////////////////////////////////////////////
CSqliteOperator::CSqliteOperator(CSqliteDb& sqlite_db)
    : _cur_bind(0), _stmt_ptr(0), _sqlite_db_ptr(0) {
  _sqlite_db_ptr = &sqlite_db;
}

CSqliteOperator::CSqliteOperator()
    : _cur_bind(0), _stmt_ptr(0), _sqlite_db_ptr(0) {}

CSqliteOperator::~CSqliteOperator(void) {
  _Finalize();
}

void CSqliteOperator::Reset() {
  _cur_bind = 0;
  if (_stmt_ptr)
    sqlite3_reset(_stmt_ptr);
}

bool CSqliteOperator::Next() {
  return SQLITE_ROW == _Step();
}

void CSqliteOperator::_Finalize() {
  _cur_bind = 0;
  if (_stmt_ptr) {
    int res = sqlite3_finalize(_stmt_ptr);
    assert(res == SQLITE_OK);
    _stmt_ptr = NULL;
  }
}

CSqliteOperator& CSqliteOperator::operator<<(int i) {
  if (_stmt_ptr) {
    sqlite3_bind_int(_stmt_ptr, ++_cur_bind, i);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator<<(unsigned char b) {
  if (_stmt_ptr) {
    sqlite3_bind_int(_stmt_ptr, ++_cur_bind, b);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator<<(short b) {
  if (_stmt_ptr) {
    sqlite3_bind_int(_stmt_ptr, ++_cur_bind, b);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator<<(float f) {
  double d = f;
  *this << d;
  return *this;
}

CSqliteOperator& CSqliteOperator::operator<<(double d) {
  if (_stmt_ptr) {
    sqlite3_bind_double(_stmt_ptr, ++_cur_bind, d);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator<<(long long i64) {
  if (_stmt_ptr) {
    sqlite3_bind_int64(_stmt_ptr, ++_cur_bind, i64);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator<<(const std::string& text) {
  if (_stmt_ptr) {
    sqlite3_bind_text(_stmt_ptr, ++_cur_bind, text.c_str(), (int)text.length(),
                      SQLITE_TRANSIENT);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator<<(Blob& blob) {
  if (_stmt_ptr) {
    sqlite3_bind_blob(_stmt_ptr, ++_cur_bind, (const void*)blob.ptr.get(),
                      blob.length, SQLITE_TRANSIENT);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator<<(StringBlob& blob) {
  if (_stmt_ptr) {
    sqlite3_bind_blob(_stmt_ptr, ++_cur_bind, (const void*)blob.data.c_str(),
                      blob.data.length(), SQLITE_TRANSIENT);
  }
  return *this;
}


CSqliteOperator& CSqliteOperator::operator<<(ConstStringBlob& blob) {
  if (_stmt_ptr) {
    sqlite3_bind_blob(_stmt_ptr, ++_cur_bind, (const void*)blob.buffer,
                      blob.len, SQLITE_TRANSIENT);
  }
  return *this;
}


CSqliteOperator& CSqliteOperator::operator>>(int& i) {
  if (_stmt_ptr) {
    i = sqlite3_column_int(_stmt_ptr, _cur_bind++);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator>>(unsigned char& b) {
  if (_stmt_ptr) {
    b = sqlite3_column_int(_stmt_ptr, _cur_bind++);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator>>(short& b) {
  if (_stmt_ptr) {
    b = sqlite3_column_int(_stmt_ptr, _cur_bind++);
  }
  return *this;
}
CSqliteOperator& CSqliteOperator::operator>>(float& f) {
  if (_stmt_ptr) {
    double d;
    d = sqlite3_column_double(_stmt_ptr, _cur_bind++);
    f = (float)d;
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator>>(double& d) {
  if (_stmt_ptr) {
    d = sqlite3_column_double(_stmt_ptr, _cur_bind++);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator>>(long long& i64) {
  if (_stmt_ptr) {
    i64 = sqlite3_column_int64(_stmt_ptr, _cur_bind++);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator>>(std::string& text) {
  if (_stmt_ptr) {
    char*  t = (char*)sqlite3_column_text(_stmt_ptr, _cur_bind++);
    if (t) {
      text.assign(t);
    }
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator>>(Blob& blob) {
  if (_stmt_ptr) {
    int data_length = sqlite3_column_bytes(_stmt_ptr, _cur_bind);
    blob.SetData((unsigned char*)sqlite3_column_blob(_stmt_ptr, _cur_bind++),
                 data_length);
  }
  return *this;
}

CSqliteOperator& CSqliteOperator::operator>>(StringBlob& blob) {
  if (_stmt_ptr) {
    int data_length = sqlite3_column_bytes(_stmt_ptr, _cur_bind);
    blob.SetData((unsigned char*)sqlite3_column_blob(_stmt_ptr, _cur_bind++),
                 data_length);
  }
  return *this;
}

// CSqliteOperator& CSqliteOperator::operator>>(CVariant& blob)
//{
//	if(_stmt_ptr)
//	{
//		int data_length = sqlite3_column_unsigned
//chars(_stmt_ptr,_cur_bind); 		blob.SetBuffer((unsigned
//char*)sqlite3_column_blob(_stmt_ptr,_cur_bind++),data_length);
//	}
//	return *this;
//}

sqlite3_stmt* CSqliteOperator::PrepareSQL(const std::string& sql) {
  _Finalize();  // Reset();
  sqlite3_stmt* statement = NULL;
  if (_sqlite_db_ptr) {
    sqlite3_prepare_v2(static_cast<sqlite3*>(*_sqlite_db_ptr), sql.c_str(),
                       (int)sql.length(), &statement, NULL);
  }
  _stmt_ptr = statement;
  return statement;
}

bool CSqliteOperator::Execute() {
  int r = _Step();
  if (r == SQLITE_BUSY) {
    r = _Step();  //考虑再给一次执行机会
  }
  // ATLASSERT(r == SQLITE_DONE);
  Reset();
  return r == SQLITE_DONE;
}

int CSqliteOperator::_Step() {
  if (!_stmt_ptr)
    return SQLITE_ERROR;
  _cur_bind = 0;
  return sqlite3_step(_stmt_ptr);
}

//////////////////////////////////////////////////////////////////////////
CSqliteDb::CSqliteDb() {
  _sqlite_db = NULL;
  _error_code = 0;
}

void CSqliteDb::Open(const char* dbPath) {
  _error_code = sqlite3_open(dbPath, &_sqlite_db);
}

int CSqliteDb::GetLastRowID() {
  return (int)sqlite3_last_insert_rowid(_sqlite_db);
}

void CSqliteDb::Close() {
  if (NULL != _sqlite_db) {
    sqlite3_close(_sqlite_db);
    _sqlite_db = NULL;
  }
}

CSqliteDb::CSqliteDb(const char* dbPath) {
  _error_code = sqlite3_open(dbPath, &_sqlite_db);
}

CSqliteDb::~CSqliteDb() {
  Close();
}

bool CSqliteDb::IsTableExist(const std::string& tb_name) {
  CSqliteOperator query(*this);
  query.PrepareSQL(("select tbl_name from sqlite_master where tbl_name = ? "));
  query << tb_name;
  return query.Next();
}
//////////////////////////////////////////////////////////////////////////

AutoTransaction::AutoTransaction(sqlite3* db_ptr, bool exclusive)
    : m_commited(false), m_db_ptr(db_ptr) {
  std::string exclusive_term = exclusive ? "EXCLUSIVE " : "";
  std::string query = "BEGIN " + exclusive_term + " TRANSACTION;";
  int exe_res;
  do {
    exe_res = sqlite3_exec((m_db_ptr), query.c_str(), 0, 0, 0);
  } while (exe_res == SQLITE_BUSY);

  if (SQLITE_OK != exe_res) {
    m_commited = true;
  }
}

AutoTransaction::~AutoTransaction() {
  if (!m_commited)
    sqlite3_exec((m_db_ptr), "ROLLBACK TRANSACTION;", 0, 0, 0);
}

void AutoTransaction::Commit() {
  if (!m_commited) {
    for (int exe_res;;) {
      exe_res = sqlite3_exec((m_db_ptr), "END TRANSACTION;", 0, 0,
                             0);  // or "COMMIT TRANSACTION"
      if (exe_res != SQLITE_BUSY)
        break;
    }
    m_commited = true;
  }
}
