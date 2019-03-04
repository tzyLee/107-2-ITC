import matplotlib.pyplot as plt
import numpy as np
from scipy import signal
from PIL import Image, ImageFilter
from mpl_toolkits.mplot3d import Axes3D
from scipy import ndimage
# from skimage.filters import prewitt_h, prewitt_v
from cv2 import filter2D

def main():
    with Image.open('img/rabbit.bmp') as img, Image.open('output/rabbit_ans.bmp') as img2:
        arr = np.array(img.rotate(90, expand=True))
        arr2 = np.array(img2)
        print("rabbit.bmp test passed:", np.all(arr == arr2))

    with Image.open('img/flower.bmp') as img, Image.open('output/flower_ans.bmp') as img2:
        img = img.convert('L')
        arr = np.array(img, dtype=np.float)
        cpp = np.array(img2)
        print("Image is grey scale:", np.all(cpp[:, :, 0][:, :, np.newaxis] == cpp))
        scipy_ndimage = prewitt_ndimage(arr)
        # skimage = prewitt_skimage(arr) ## TODO find the bug
        scipy = prewitt_scipy(arr)
        scipy_complex = prewitt_scipy_complex(arr)
        # pil = prewitt_pil(img) ## TODO find the bug
        numpy = prewitt_numpy(arr)
        cv2 = prewitt_cv2(arr)
        # Compare cpp and cv2
        compare(cv2, cpp[:, :, 0])
        compare(cv2, scipy_ndimage)
        compare(cv2, scipy)
        compare(cv2, scipy_complex)
        compare(cv2, numpy)
        show_array((2, 3), cpp, scipy_ndimage, scipy, scipy_complex, numpy, cv2,
                        titles=("cpp", "scipy_ndimage", "scipy", "scipy_complex", "numpy", "cv2"))

def compare(ref, to_test):
    ref = ref.astype(np.int32, casting='safe')
    print("flower.bmp test passed:", np.all(ref == to_test))
    print("mean(abs(error)):", np.mean(np.abs(ref-to_test)))
    print("max(abs(error)):", np.max(np.abs(ref-to_test)))
    # plt.imshow(Image.fromarray(np.abs(ref-to_test)))

def array_to_image(arr):
    if type(arr) is np.ndarray:
        if len(arr.shape) == 2:
            return arr.repeat(3, axis=-1).reshape(*arr.shape, 3) # Repeat same value for RGB, assume array is in grey scale
        else:
            assert len(arr.shape) == 3, "Shape is not (Width, Height, Color)"
            return arr
    assert type(arr) is Image, "Unknown type of array `{}`".format(type(arr))
    return arr

def show_array(shape, *arrs, titles=None):
    assert np.prod(shape) == len(arrs), "Product of shape does not match the number of arrays to show"
    assert len(arrs) == len(titles), "Must provide the same number of title as arrays"
    arrs = map(array_to_image, arrs)
    _, axes = plt.subplots(*shape)
    for ax, image in zip(axes.flatten(), arrs):
        ax.imshow(image)
    if titles:
        for ax, title in zip(axes.flatten(), titles):
            ax.set_title(title)
    plt.show()

def set_border(array, value):
    array[0, :] = value
    array[-1, :] = value
    array[:, 0] = value
    array[:, -1] = value

def rotate_numpy(array):
    return np.transpose(array[:, ::-1, :], axes=(1, 0, 2))

def rotate_pil(image):
    return image.rotate(90, expand=True)

def prewitt_ndimage(arr):
    UINT8 = np.iinfo(np.uint8)
    x = ndimage.prewitt(arr, axis=0)
    y = ndimage.prewitt(arr)
    temp = np.hypot(x, y).clip(UINT8.min, UINT8.max).astype(np.uint8)
    set_border(temp, 255)
    return temp

# def prewitt_skimage(arr):
#     ## TODO find the bug
#     UINT8 = np.iinfo(np.uint8)
#     temp = np.hypot(prewitt_h(arr), prewitt_v(arr))
#     set_border(temp, 255)
#     return temp.clip(UINT8.min, UINT8.max).astype(np.uint8)

def prewitt_scipy(arr):
    g_y = np.array([-1, 0, 1]).repeat(3).reshape(3, 3)
    g_x = g_y.T[::-1]
    x = signal.convolve2d(arr, g_x, mode='same')
    y = signal.convolve2d(arr, g_y, mode='same')
    UINT8 = np.iinfo(np.uint8)
    temp = np.hypot(x, y).clip(UINT8.min, UINT8.max).astype(np.uint8)
    set_border(temp, 255)
    return temp

def prewitt_scipy_complex(arr):
    g_y = np.array([-1, 0, 1]).repeat(3).reshape(3, 3)
    g_x = g_y.T*1j
    convolution = signal.convolve2d(arr, g_x+g_y, mode='same')
    UINT8 = np.iinfo(np.uint8)
    temp = np.absolute(convolution).clip(UINT8.min, UINT8.max).astype(np.uint8)
    set_border(temp, 255)
    return temp

def prewitt_cv2(arr):
    g_y = np.array([-1, 0, 1]).repeat(3).reshape(3, 3)
    g_x = g_y.T[::-1]
    UINT8 = np.iinfo(np.uint8)
    x = filter2D(arr, ddepth=-1, kernel=g_x).astype(np.float)
    y = filter2D(arr, ddepth=-1, kernel=g_y).astype(np.float)
    temp = np.hypot(x, y)
    set_border(temp, 255)
    return temp.clip(UINT8.min, UINT8.max).astype(np.uint8)

# def prewitt_pil(image):
#     ## TODO find the bug
#     G_x = ImageFilter.Kernel((3, 3), (-1,  0,  1,
#                                       -1,  0,  1,
#                                       -1,  0,  1), scale=1)
#     G_y = ImageFilter.Kernel((3, 3), (-1, -1, -1,
#                                        0,  0,  0,
#                                        1,  1,  1), scale=1)
#     x = image.filter(G_x)
#     y = image.filter(G_y)
#     temp = np.sqrt(np.array(x, dtype=np.float)**2 + np.array(y, dtype=np.float)**2)
#     UINT8 = np.iinfo(np.uint8)
#     set_border(temp, 255)
#     return temp.clip(UINT8.min, UINT8.max).astype(np.uint8)

def prewitt_numpy(arr):
    g_y = np.array([-1, 0, 1]).repeat(3).reshape(3, 3)
    g_x = g_y.T*1j
    kernel = g_x + g_y
    result = np.full(arr.shape, 0, dtype=np.complex128)
    loss = kernel.shape[0]//2
    slicing = np.s_[loss:-loss, loss:-loss] # assume kernel is square matrix, and have odd number of row
    diff = kernel.shape[0] - 1
    for (i, j), weight in np.ndenumerate(kernel):
        result[slicing] += weight*arr[i:(i-diff or None), j:(j-diff or None)]
    UINT8 = np.iinfo(np.uint8)
    result = np.absolute(result).clip(UINT8.min, UINT8.max).astype(np.uint8)
    set_border(result, 255)
    return result


if __name__ == '__main__':
    main()