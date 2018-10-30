## Installing

Place the **shingine** folder into 

```
$HOUDINI_USER_PREF_DIR/python2.7libs
```

Create a new shelf tool with the following code:

```
import shingine
shingine.save_scene()
```

To export selected meshes use:

```
shingine.save_selected_meshes()
```