import logging
import os

import pytest
from pytest_embedded_idf.dut import IdfDut

@pytest.mark.supported_targets
@pytest.mark.generic
def test_fc2(dut: IdfDut) -> None:
    binary_file = os.path.join(dut.app.binary_path, "fc2.bin")
    bin_size = os.path.getsize(binary_file)
    logging.info("fc2_bin_size: {}KB".format(bin_size // 1024))
    assert False, "Failed to run test blink"
