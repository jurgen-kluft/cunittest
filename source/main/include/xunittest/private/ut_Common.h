#ifndef __XUNITTEST_COMMON_H__
#define __XUNITTEST_COMMON_H__




namespace UnitTest
{
	template<typename ElementType>
	class Array
	{
	public:
		int size()
		{
			return mArrayNum;
		}

		void clear()
		{

		}

		void pushBack(const ElementType & inItem)
		{

		}

		void erase(int inIdx)
		{

		}


	protected:
	
		int				mArrayNum;
		int				mArrayMax;
		ElementType *	mArray;
	};

	template<typename ElementType>
	class List
	{
	public:

		class ListNode
		{
		public:
			ListNode * getNext();
			ListNode * getPrev();
			ElementType & getValue();


		protected:
			friend class List<ElementType>;

			ListNode *		mPrev;
			ListNode *		mNext;
			ElementType		mValue;	
		};

		void pushBack(const ElementType & inItem);

	protected:
		
		int				mListNum;
		ListNode *		mList;
	};
}

#endif //__XUNITTEST_COMMON_H__