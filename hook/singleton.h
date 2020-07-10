#ifndef __Hook_Singleton_H__
#define __Hook_Singleton_H
namespace hook{
	template<typename T>
	class Singleton{
	public:
		static T& get_instance(){
			static T instance;
			return instance;
		}
		virtual ~Singleton(){}
		Singleton(const Singleton&)=delete; //ban copy function 
		Singleton& operator =(const Singleton&)=delete;//ban basic assignment 
	protected:
		Singleton(){}
	};	
	
	template<class T>
	class SingletonPtr{
	public:
		static std::shared_ptr<T> get_instance(){
			static std::shared_ptr<T> instance(new T);
			return instance;
		}
		virtual ~SingletonPtr(){}
		SingletonPtr(const SingletonPtr&)=delete; //ban copy function 
		SingletonPtr& operator =(const SingletonPtr&)=delete;//ban basic assignment 
	protected:
		SingletonPtr(){}
	};


	class DerivedSinglePtr: public SingletonPtr<DerivedSinglePtr>{
		friend class SingletonPtr<DerivedSinglePtr>;
	public:
		DerivedSinglePtr(const DerivedSinglePtr&)=delete;
		DerivedSinglePtr& operator = (const DerivedSinglePtr&)=delete;
	private:
		DerivedSinglePtr()=default;
	};







	class DerivedSingle: public Singleton<DerivedSingle>{
		friend class Singleton<DerivedSingle>;
	public:
		DerivedSingle(const DerivedSingle&)=delete;
		DerivedSingle& operator = (const DerivedSingle&)=delete;
	private:
		DerivedSingle()=default;
	};




}
#endif