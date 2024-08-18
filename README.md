a us level timer

## TDOO

+ vectorization
+ coroutine
+ stl style rbtree
+ consider execption such as local clock change/debug
+ need add in order

 ## Confuse
 +   ```_current_timer.getControlStg()->strategy(_current_timer);```  调用返回用2-3us?

 +  循环sleep时 如果不做些操作 os不会给你高优先级