#include "wvserialize.h"

struct Silly
{
    int big;
    long bigger;
    short small;
    WvString s1;
    bool b;
    WvString s2;
    
    void print()
    {
	printf("Silly: %d %ld %d '%s' %d '%s'\n",
	       big, bigger, small, s1.cstr(), b, s2.cstr());
    }
};


void _wv_serialize(WvBuf &buf, const Silly &s)
{
    wv_serialize(buf, s.big);
    wv_serialize(buf, (int)s.bigger);
    wv_serialize(buf, s.small);
    wv_serialize(buf, s.s1);
    wv_serialize(buf, s.b);
    wv_serialize(buf, s.s2);
}


template <>
Silly *_wv_deserialize<Silly *>(WvBuf &buf)
{
    Silly *s = new Silly;
    s->big    = wv_deserialize<int>(buf);
    s->bigger = wv_deserialize<int>(buf);
    s->small  = wv_deserialize<short>(buf);
    s->s1     = wv_deserialize<WvString>(buf);
    s->b      = wv_deserialize<int>(buf);
    s->s2     = wv_deserialize<WvString>(buf);
    return s;
}

template <>
Silly _wv_deserialize<Silly>(WvBuf &buf)
{
    Silly *tmp = wv_deserialize<Silly *>(buf);
    Silly tmp2 = *tmp;
    delete tmp;
    return tmp2;
}


/**
 * Serialize a list of serializable things.
 * 
 * Oh boy - I think I'm having a bit too much fun.
 */
template <typename T>
void _wv_serialize(WvBuf &buf, WvList<T> &list)
{
    // save the number of elements
    _wv_serialize(buf, (size_t)list.count());
    
    // save the elements
    typename WvList<T>::Iter i(list);
    for (i.rewind(); i.next(); )
	_wv_serialize(buf, *i);
}


/** Deserialize a list of serializable things. */
template <typename T>
class WvDeserialize<WvList<T> *>
{
public:
    static WvList<T> *go(WvBuf &buf)
    {
	WvList<T> *list = new WvList<T>;
	size_t nelems = wv_deserialize<size_t>(buf);
	
	for (size_t count = 0; count < nelems; count++)
	{
	    T t = wv_deserialize<T>(buf);
	    list->append(new T(t), true);
	}
	
	return list;
    }
};


DeclareWvList(int);


int main()
{
    WvDynBuf buf;
    bool bb = false;
    
    wv_serialize(buf, "hello");
    wv_serialize(buf, 5U);
    wv_serialize(buf, (short)7);
    wv_serialize(buf, true);
    wv_serialize(buf, bb);
    wv_serialize(buf, 'x');
    printf("buf used: %d\n", buf.used());
    
    printf("got: '%s'\n", wv_deserialize<WvString>(buf).cstr());
    printf("buf used: %d\n", buf.used());
    printf("got: %d\n", wv_deserialize<int>(buf));
    printf("got: %d\n", wv_deserialize<short>(buf));
    printf("got: %d\n", wv_deserialize<int>(buf));
    printf("got: %d\n", wv_deserialize<int>(buf));
    printf("got: '%c'\n", wv_deserialize<char>(buf));
    printf("\n");
    
    buf.zap();
    Silly s = {5, 6, 7, "hello", false, "world"};
    s.print();
    wv_serialize(buf, s);
    s.print();
    printf("buf used: %d\n", buf.used());

    WvBufCursor buf2(buf, 0, buf.used());
    Silly *s2 = wv_deserialize<Silly *>(buf2);
    s2->print();
    delete s2;
    printf("buf used: %d/%d\n", buf2.used(), buf.used());
    
    Silly s3 = wv_deserialize<Silly>(buf);
    s3.print();
    
    
    buf.zap();
    intList list;
    list.append(new int(7), true);
    list.append(new int(5), true);
    list.append(new int(1231), true);
    list.append(new int(973), true);
    wv_serialize(buf, list);
    printf("buf used: %d\n", buf.used());
    
    intList *list2 = wv_deserialize<intList *>(buf);
    printf("%d list elements after deserialize.\n", list.count());
    intList::Iter i(*list2);
    for (i.rewind(); i.next(); )
	printf(" ... %d\n", *i);
    delete list2;
    
    return 0;
}
