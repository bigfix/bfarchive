#ifndef BigFix_Stream_h
#define BigFix_Stream_h

namespace BigFix
{

class DataRef;

class Stream
{
public:
  virtual ~Stream();

  virtual void Write( DataRef ) = 0;
  virtual void End() = 0;
};

class NullStream : public Stream
{
public:
  virtual void Write( DataRef );
  virtual void End();
};

class AppendStream : public Stream
{
public:
  AppendStream();

  void Reset( Stream& );

  virtual void Write( DataRef );
  virtual void End();

private:
  Stream* m_output;
};

void WriteAndEnd( Stream&, DataRef );

}

#endif
