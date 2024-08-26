using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LerpSound : MonoBehaviour
{
    AudioSource source;

    Coroutine lerpCoroutine;

    void Awake()
    {
        source = GetComponent<AudioSource>();
    }

    public void LerpTo(float value, float time = 1f)
    {
        if (lerpCoroutine != null)
        {
            StopCoroutine(lerpCoroutine);
        }
        lerpCoroutine = StartCoroutine(LerpVolume(value, time));
    }

    private IEnumerator LerpVolume(float targetVolume, float duration)
    {
        float startVolume = source.volume;
        float elapsed = 0f;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            source.volume = Mathf.Lerp(startVolume, targetVolume, elapsed / duration);
            yield return null; // Ждем следующий кадр
        }

        source.volume = targetVolume; // Убедимся, что устанавливаем конечное значение
    }
}
