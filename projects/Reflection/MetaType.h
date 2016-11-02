#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>

#ifndef META_TYPES
#define META_TYPES

#define MAX_STRING_SIZE = 256;

struct MetaType;

template <typename To> To Cast(void* v, const MetaType& m);

struct MetaType
{
	virtual const char* Name() const = 0;
	virtual size_t SizeOf() const = 0;
	virtual double ToNumber(void* v) const = 0;
	virtual const char* ToString(void* v) const = 0;
	virtual void Cast(void* to, void* from, const MetaType& fromType) const = 0;
	virtual void Serialize(char *name, void *data) = 0;
	virtual void Deserialize(char *name, void *data) = 0;

};

/************************************************************************/
/*	Void                                                                 /
/************************************************************************/
struct VoidMetaType : public MetaType
{
	virtual const char* Name() const { return "void"; }
	virtual size_t SizeOf() const { return 0; }
	virtual double ToNumber(void* v) const { return 0; }
	virtual const char* ToString(void* v) const { return NULL; }
	virtual void Cast(void* to, void* from, const MetaType& fromType) const {}
	virtual void Serialize(char *name, void *data) {}
	virtual void Deserialize(char *name, void *data) {}

} g_voidMetaType;

/************************************************************************/
/*	Integer                                                              /
/************************************************************************/
struct IntMetaType : public MetaType
{
	virtual const char* Name() const { return "int"; }
	virtual size_t SizeOf() const { return sizeof(int); }
	virtual double ToNumber(void* v) const { return *(int*)v; }

	virtual const char* ToString(void* v) const {
		static char buf[65];
		sprintf(buf, "%d", *(int*)v);
		return buf;
	}

	virtual void Cast(void* to, void* from, const MetaType& fromType) const {
		*(int*)to = (int)fromType.ToNumber(from);
	}

	virtual void Serialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream O_stream (dir, std::ios::out | std::ios::binary );

		O_stream.write((char*)data, sizeof(int));
		O_stream.close();
	}

	virtual void Deserialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream I_stream (dir, std::ios::in | std::ios::binary );

		I_stream.read((char*)data, sizeof(int));
		I_stream.close();
	}

} g_intMetaType;

/************************************************************************/
/*	String                                                               /
/************************************************************************/
struct CharStarMetaType : public MetaType
{
	virtual const char* Name() const { return "char*"; }
	virtual size_t SizeOf() const { return sizeof(char*); }
	virtual double ToNumber(void* v) const { return atoi(*(char**)v); }
	virtual const char* ToString(void* v) const { return *(char**)v; }

	virtual void Cast(void* to, void* from, const MetaType& fromType) const {
		*(const char**)to = (const char*)fromType.ToString(from);
	}

    virtual void Serialize(char *name, void *data) {
    	char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream O_stream (dir, std::ios::out | std::ios::binary );

		O_stream.write((char*)data, sizeof((char*)data));
		O_stream.close();
    }

	virtual void Deserialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream I_stream (dir, std::ios::in | std::ios::binary );

		I_stream.read((char*)data, sizeof((char*)data));
		I_stream.close();
	}

} g_charstarMetaType;


/************************************************************************/
/*	Float                                                                /
/************************************************************************/
struct floatMetaType : public MetaType
{
	virtual const char* Name() const { return "float"; }
	virtual size_t SizeOf() const { return sizeof(float); }
	virtual double ToNumber(void* v) const { return *(float*)v; }

	virtual const char* ToString(void* v) const {
		static char buf[65];
		sprintf(buf, "%f", *(float*)v);
		return buf;
	}

	virtual void Cast(void* to, void* from, const MetaType& fromType) const {
		*(float*)to = (float)fromType.ToNumber(from);
	}

    virtual void Serialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream O_stream (dir, std::ios::out | std::ios::binary );

		O_stream.write((char*)data, sizeof(float));
		O_stream.close();
	}

	virtual void Deserialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream I_stream (dir, std::ios::in | std::ios::binary );

		I_stream.read((char*)data, sizeof(float));
		I_stream.close();
	}

} g_floatMetaType;

/************************************************************************/
/*	Double                                                               /
/************************************************************************/
struct doubleMetaType : public MetaType
{
	virtual const char* Name() const { return "double"; }
	virtual size_t SizeOf() const { return sizeof(double); }
	virtual double ToNumber(void* v) const { return *(double*)v; }

	virtual const char* ToString(void* v) const {
		static char buf[65];
		sprintf(buf, "%f", *(double*)v);
		return buf;
	}

	virtual void Cast(void* to, void* from, const MetaType& fromType) const {
		*(double*)to = (double)fromType.ToNumber(from);
	}

    virtual void Serialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream O_stream (dir, std::ios::out | std::ios::binary );

		O_stream.write((char*)data, sizeof(double));
		O_stream.close();
	}

	virtual void Deserialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream I_stream (dir, std::ios::in | std::ios::binary );

		I_stream.read((char*)data, sizeof(double));
		I_stream.close();
	}

} g_doubleMetaType;

/************************************************************************/
/*	Short                                                                /
/************************************************************************/
struct shortMetaType : public MetaType
{
	virtual const char* Name() const { return "short"; }
	virtual size_t SizeOf() const { return sizeof(short); }
	virtual double ToNumber(void* v) const { return *(short*)v; }

	virtual const char* ToString(void* v) const{
		static char buf[65];
		sprintf(buf, "%d", *(short*)v);
		return buf;
	}

	virtual void Cast(void* to, void* from, const MetaType& fromType) const {
		*(short*)to = (short)fromType.ToNumber(from);
	}

    virtual void Serialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream O_stream (dir, std::ios::out | std::ios::binary );

		O_stream.write((char*)data, sizeof(short));
		O_stream.close();
	}

	virtual void Deserialize(char *name, void *data) {
		char dir[256] = "Serials/";
		strcat(dir, name);

		std::fstream I_stream (dir, std::ios::in | std::ios::binary );

		I_stream.read((char*)data, sizeof(short));
		I_stream.close();
	}

} g_shortMetaType;

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

MetaType& GetMetaType(int)         { return g_intMetaType;      }
MetaType& GetMetaType(const char*) { return g_charstarMetaType; }
MetaType& GetMetaType(float)       { return g_floatMetaType;    }
MetaType& GetMetaType(void)        { return g_voidMetaType;     }
MetaType& GetMetaType(double)      { return g_doubleMetaType;   }
MetaType& GetMetaType(short)       { return g_shortMetaType;    }


template <typename T>
const MetaType& GetMetaTypeByType() { T foo; return GetMetaType(foo); }

template <>
const MetaType& GetMetaTypeByType<void>() { return g_voidMetaType; }

template <typename To>
To Cast(void* v, const MetaType& m)
{
	To t;
	GetMetaType(t).Cast(&t, v, m);
	return t;
}

#endif

