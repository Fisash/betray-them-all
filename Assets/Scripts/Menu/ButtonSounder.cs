using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class ButtonSounder : MonoBehaviour, IPointerEnterHandler
{
    public AudioClip hoverSound;
    public AudioClip clickSound;

    private AudioSource audioSource;

    void Start()
    {
        audioSource = GameObject.Find("SoundManager").GetComponent<AudioSource>();
        GetComponent<Button>().onClick.AddListener(Click);
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if(hoverSound!=null)
            audioSource.PlayOneShot(hoverSound);
    }

    public void Click()
    {
        if (clickSound != null)
            audioSource.PlayOneShot(clickSound);
    }

}