# import subprocess

# subprocess.run(['pybind11-stubgen', '-o../../build-MSVC2019-Release/src/h5gtpy/Debug', '_h5gt'])  # NO SPACE IN -oPATH!
# subprocess.run(['pybind11-stubgen', '-o../../build-MSVC2019-Release/src/h5gtpy/Debug', 'h5gtpy._h5gt'])  # NO SPACE IN -oPATH!
# subprocess.run(['pybind11-stubgen', '-obuild-MSVC2019-Release/src/h5gtpy/Debug', 'h5gtpy'])  # NO SPACE IN -oPATH!

import numpy as np
from h5gtpy import h5gt
from enum import Flag, auto     # allow bitwise operations on enum
import os.path
import unittest
import pathlib

class test_h5gtpy(unittest.TestCase):
    def setUp(self):
        global trig
        if not trig:
            pathlib.Path('tmp').mkdir(exist_ok=True)

        trig = True

    def test_file_creation(self):
        file_name = 'tmp/test_file_creation.h5'
        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))
        self.assertTrue(os.path.isfile(file_name))

    def test_object_creation(self):
        file_name = 'tmp/test_object_creation.h5'
        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        group_name = 'g'
        group = file.createGroup(group_name)
        self.assertTrue(file.exist(group.getPath()))

        dset_name = 'data'
        dset = file.createDataSet(dset_name, 5, h5gt.AtomicDouble())
        self.assertTrue(file.exist(dset.getPath()))

        attr_name = 'attr'
        attr = dset.createAttribute('attr_name', 5, h5gt.AtomicStr())
        self.assertTrue(dset.hasAttribute(attr.getName()))

    def test_node_functionality(self):
        file_name = 'tmp/test_node_functionality.h5'
        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        dset1 = file.createDataSet('data1', h5gt.DataSpace([3, 5]), h5gt.AtomicDouble())
        self.assertTrue(file.exist(dset1.getPath()))

        dset2 = file.createDataSet('data2', [3, 5], h5gt.AtomicDouble())
        self.assertTrue(file.exist(dset2.getPath()))

        dset3 = file.createDataSet('data3', 5, h5gt.AtomicDouble())
        self.assertTrue(file.exist(dset3.getPath()))

        dset4 = h5gt.DataSet.FromId(dset3.getId(True))
        self.assertTrue(dset3 == dset4)
        self.assertTrue(dset3 != dset1)

        obj_names = file.listObjectNames()
        self.assertListEqual(obj_names, ['data1', 'data2', 'data3'])

        self.assertEqual(file.getNumberObjects(), 3)

        self.assertEqual(file.getLinkType(dset1.getPath()), h5gt.LinkType.Hard)

        file.createAttribute('attr', 5, h5gt.AtomicStr())
        self.assertEqual(file.getNumberAttributes(), 1)

        group = file.createGroup('/path/to/group')
        self.assertTrue(file.exist(group.getPath()))

        file.unlink(group.getPath())
        self.assertFalse(file.hasObject('/path/to/group', h5gt.ObjectType.Group))

        dlink_soft = file.createLink(dset1, 'dlink_soft', h5gt.LinkType.Soft)
        self.assertEqual(dlink_soft.getLinkInfo().getLinkType(), h5gt.LinkType.Soft)

        dlink_hard = file.createLink(dset1, 'dlink_hard', h5gt.LinkType.Hard)
        self.assertEqual(dlink_hard.getLinkInfo().getLinkType(), h5gt.LinkType.Hard)

        file_name2 = 'tmp/test_node_functionality_ext_link.h5'
        file2 = h5gt.File(file_name2, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        flink_ext = file2.createLink(file, 'external_file', h5gt.LinkType.External)
        self.assertTrue(file != file2)

    def test_read_write_dataset(self):
        file_name = 'tmp/test_read_write_dataset.h5'

        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        dset = file.createDataSet('data_int_scalar', 1, h5gt.AtomicInt())
        dset.write(17)
        self.assertTrue(dset.readInt() == 17)

        dset = file.createDataSet('data_int_list', [1, 3], h5gt.AtomicInt())
        dset.write([0, 1, 2])
        self.assertListEqual(dset.readInt()[0].tolist(), [0, 1, 2])

        #---------------------------------------------------------------------------------------------------------------
        #--------------------------CHECK `WRITE` METHOD-----------------------------------------------------------------
        #---------------------------------------------------------------------------------------------------------------
        # also check if order 'C' or 'F' matters
        m = np.array([[0, 1], [2, 3], [4, 5]], dtype=np.int32, order='C')
        dset = file.createDataSet('data_int_np_order_C', list(m.shape), h5gt.AtomicInt())
        dset.write(m)
        mm = dset.readInt()
        self.assertListEqual(m.tolist(), mm.tolist())

        m = np.array([[0, 1], [2, 3], [4, 5]], dtype=np.int32, order='F')
        dset = file.createDataSet('data_int_np_order_F', list(m.shape), h5gt.AtomicInt())
        dset.write(m)
        mm = dset.readInt()
        self.assertListEqual(m.tolist(), mm.tolist())

        # check `write`
        m = np.array([[0.0, 1.1], [2.2, 3.3], [4.4, 5.5]], dtype=np.float64)
        dset = file.createDataSet('data_double_np', list(m.shape), h5gt.AtomicDouble())
        dset.write(m)
        mm = dset.readDouble()
        self.assertListEqual(m.tolist(), mm.tolist())

        m = np.array([[0+0j, 1-1j], [2+2j, 3-3j], [4+4j, 5-5j]], dtype=np.complex64)
        dset = file.createDataSet('data_cdouble_np', list(m.shape), h5gt.AtomicCDouble())
        dset.write(m)
        mm = dset.readCDouble()
        self.assertListEqual(m.tolist(), mm.tolist())

        #---------------------------------------------------------------------------------------------------------------
        #--------------------------CHECK `WRITE_RAW` METHOD-------------------------------------------------------------
        #---------------------------------------------------------------------------------------------------------------
        # also check if order 'C' or 'F' matters (you can see it does matter)
        m = np.array([[0, 1], [2, 3], [4, 5]], dtype=np.int32, order='C')
        dset = file.createDataSet('data_int_np_order_C_write_raw', list(m.shape), h5gt.AtomicInt())
        dset.write_raw(m)
        mm = dset.readInt()
        self.assertListEqual(m.tolist(), mm.transpose().reshape(m.shape).tolist())

        m = np.array([[0, 1], [2, 3], [4, 5]], dtype=np.int32, order='F')
        dset = file.createDataSet('data_int_np_order_F_write_raw', list(m.shape), h5gt.AtomicInt())
        dset.write_raw(m)
        mm = dset.readInt()
        self.assertListEqual(m.tolist(), mm.tolist())

        # check `write_raw`
        m = np.array([[0.0, 1.1], [2.2, 3.3], [4.4, 5.5]], dtype=np.float64, order='F')
        dset = file.createDataSet('data_double_np_write_raw', list(m.shape), h5gt.AtomicDouble())
        dset.write_raw(m)
        mm = dset.readDouble()
        self.assertListEqual(m.tolist(), mm.tolist())

        m = np.array([[0+0j, 1-1j], [2+2j, 3-3j], [4+4j, 5-5j]], dtype=np.complex64, order='F')
        dset = file.createDataSet('data_cdouble_np_write_raw', list(m.shape), h5gt.AtomicCDouble())
        dset.write_raw(m)
        mm = dset.readCDouble()
        self.assertListEqual(m.tolist(), mm.tolist())

        #---------------------------------------------------------------------------------------------------------------
        #--------------------------CHECK `WRITE` STRING-----------------------------------------------------------------
        #---------------------------------------------------------------------------------------------------------------
        s = 'sometimes I prefer heavy music'
        dset = file.createDataSet('data_str', 1, h5gt.AtomicStr())
        dset.write(s)
        ss = dset.readStr()
        self.assertEqual(s, ss[0])

        s = ['T', 'I', 'E', 'R', 'R', 'A']
        dset = file.createDataSet('data_str_list', len(s), h5gt.AtomicStr())
        dset.write(s)
        ss = dset.readStr()
        self.assertListEqual(s, ss)

    def test_read_write_attribute(self):
        file_name = 'tmp/test_read_write_attribute.h5'

        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        attr = file.createAttribute('attr_int_scalar', 1, h5gt.AtomicInt())
        attr.write(17)
        self.assertTrue(attr.readInt() == 17)

        attr = file.createAttribute('attr_int_list', 3, h5gt.AtomicInt())
        attr.write([0, 1, 2])
        self.assertListEqual(attr.readInt().flatten().tolist(), [0, 1, 2])

        v = np.array([0.0, 1.1], dtype=np.float64)
        attr = file.createAttribute('attr_double_np', v.size, h5gt.AtomicDouble())
        attr.write(v)
        vv = attr.readDouble()
        self.assertListEqual(v.tolist(), vv.flatten().tolist())

        v = np.array([0.0, 1.1], dtype=np.float64)
        attr = file.createAttribute('attr_double_np_raw', v.size, h5gt.AtomicDouble())
        attr.write_raw(v)
        vv = attr.readDouble()
        self.assertListEqual(v.tolist(), vv.flatten().tolist())

        #---------------------------------------------------------------------------------------------------------------
        #--------------------------CHECK `WRITE` STRING-----------------------------------------------------------------
        #---------------------------------------------------------------------------------------------------------------
        s = 'but mostly I like classic rock'
        attr = file.createAttribute('attr_str', 1, h5gt.AtomicStr())
        attr.write(s)
        ss = attr.readStr()
        self.assertEqual(s, ss[0])

        s = ['C', 'O', 'L', 'A', 'D', 'A']
        attr = file.createAttribute('attr_str_list', len(s), h5gt.AtomicStr())
        attr.write(s)
        ss = attr.readStr()
        self.assertListEqual(s, ss)

    def test_exception(self):
        file_name = 'tmp/test_exception.h5'

        file = h5gt.File(file_name, h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))

        dname = 'data'
        dset = file.createDataSet(dname, 3, h5gt.AtomicFloat())

        self.assertRaises(h5gt.Exception, file.createDataSet, dname, 3, h5gt.AtomicFloat())


if __name__ == '__main__':
    unittest.main()
