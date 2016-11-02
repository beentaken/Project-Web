#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AutoLister.h"
#include "MetaType.h"

#define META_REGISTER_FUN(f) \
	MetaFunction g_metaFunction_##f(#f, f)

struct Variable
{
	Variable() : v(0), m(&GetMetaType()) {}

	template <typename T>
	Variable(const T& t) : v((void*)&t), m(&GetMetaType(t)) {}

	void* v;
	const MetaType* m;
};

class FunctionSignature
{
public:

	template <typename Ret>
	FunctionSignature(Ret (*) ())
	{
		m_ret = &GetMetaTypeByType<Ret>();
		m_args = 0;
		m_argCount = 0;
	}

	template <typename Ret, typename Arg0>
	FunctionSignature(Ret (*) (Arg0))
	{
		m_ret = &GetMetaTypeByType<Ret>();
		static const MetaType* args[] = { &GetMetaTypeByType<Arg0>() };
		m_args = args;
		m_argCount = 1;
	}

	template <typename Ret, typename Arg0, typename Arg1>
	FunctionSignature(Ret (*) (Arg0, Arg1))
	{
		m_ret = &GetMetaTypeByType<Ret>();
		static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>() };
		m_args = args;
		m_argCount = 2;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
	FunctionSignature(Ret (*) (Arg0, Arg1, Arg2))
	{
		m_ret = &GetMetaTypeByType<Ret>();
		static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>() };
		m_args = args;
		m_argCount = 3;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
	FunctionSignature(Ret (*) (Arg0, Arg1, Arg2, Arg3))
	{
		m_ret = &GetMetaTypeByType<Ret>();
		static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>() };
		m_args = args;
		m_argCount = 4;
	}

	template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	FunctionSignature(Ret (*) (Arg0, Arg1, Arg2, Arg3, Arg4))
	{
		m_ret = &GetMetaTypeByType<Ret>();
		static const MetaType* args[] = { &GetMetaTypeByType<Arg0>(), &GetMetaTypeByType<Arg1>(), &GetMetaTypeByType<Arg2>(), &GetMetaTypeByType<Arg3>(), &GetMetaTypeByType<Arg4>() };
		m_args = args;
		m_argCount = 5;
	}

	const MetaType* RetType() const { return m_ret; }
	const MetaType* ArgType(int idx) const { return m_args[idx]; }
	int ArgCount() const { return m_argCount; }

private:
	const MetaType* m_ret;
	const MetaType** m_args;
	int m_argCount;

};

/////////////////////////////////////////////////////////////////////////////////
// Apply Implementation /////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
template <typename Ret, typename Arg0>
void Apply(Ret (* fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	Ret r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0>
void Apply(void (* fun) (Arg0), Variable ret, Variable* args, int argCount)
{
	int r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}

template <typename Ret, typename Arg0, typename Arg1>
void Apply(Ret (* fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	Ret r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1>
void Apply(void (* fun) (Arg0, Arg1), Variable ret, Variable* args, int argCount)
{
	int r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2>
void Apply(Ret (* fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	Ret r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2>
void Apply(void (* fun) (Arg0, Arg1, Arg2), Variable ret, Variable* args, int argCount)
{
	int r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Apply(Ret (* fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	Ret r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3>
void Apply(void (* fun) (Arg0, Arg1, Arg2, Arg3), Variable ret, Variable* args, int argCount)
{
	int r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}

template <typename Ret, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Apply(Ret (* fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	Ret r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaType(r));
}

template <typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
void Apply(void (* fun) (Arg0, Arg1, Arg2, Arg3, Arg4), Variable ret, Variable* args, int argCount)
{
	int r ;//= fun(Cast<Ret>(args[0]));
	ret.m->Cast(ret.v, &r, GetMetaTypeByType<void>());
}
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

template <typename Fun>
void ApplyWrapper(void (*fun) (), Variable ret, Variable* args, int argCount)
{
	Apply((Fun)fun, ret, args, argCount);
}

struct MetaFunction : public AutoLister<MetaFunction>
{
public:
	template <typename Fun>
	MetaFunction(const char* name, Fun fun)
		: m_name(name)
		, m_sig(fun)
		, m_fun((void (*) ())fun)
		, m_applyWrapper(ApplyWrapper<Fun>)
	{}

	const char* Name() const;
	
	const MetaType* RetType() const { return m_sig.RetType(); }
	const MetaType* ArgType(int idx) const { return m_sig.ArgType(idx); }
	int ArgCount() const { return m_sig.ArgCount(); }

	void Apply(Variable ret, Variable* args, int argCount) const
	{
		m_applyWrapper(m_fun, ret, args, argCount);
	}

	FunctionSignature& GetSignature() { return m_sig; }
	const char* GetName() { return m_name; }

private:
	const char* m_name;
	FunctionSignature m_sig;
	void (* m_fun) ();
	void (* m_applyWrapper) (void (* fun) (), Variable, Variable*, int);
};

void MetaPrintFunctions(std::ostream& os)
{
	for (MetaFunction* mf = MetaFunction::Head(); mf; mf = mf->Next())
	{
		os << "\n";
		os << "Name        : " << mf->GetName() << "\n";
		os << "Return Type : " << mf->GetSignature().RetType()->Name() << "\n";

		for(int i = 1; i <= mf->GetSignature().ArgCount(); i++)
			os << "Arg" << i << " Type   : " << mf->GetSignature().ArgType(i - 1)->Name() << "\n";

		os << "Arg count   : " << mf->GetSignature().ArgCount() << "\n";

	}
}
