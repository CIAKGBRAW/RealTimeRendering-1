//
//  MyView.m
//  HelloWorld
//
//  Created by user138307 on 6/3/18.
//


#import "MyView.h"

@implementation MyView
{
    NSString * centralText;
}

- (id)initWithFrame:(CGRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if(self)
    {
        
        [self setBackgroundColor:[UIColor whiteColor]];
        
        centralText=@"HelloWorld !!";
        
        UITapGestureRecognizer *singleTapGestureRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingletap:)];
        [singleTapGestureRecognizer setNumberOfTapsRequired:1];
        [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [singleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:singleTapGestureRecognizer];
        
        UITapGestureRecognizer *doubleTapGestureRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
        [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
        [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
        [doubleTapGestureRecognizer setDelegate:self];
        [self addGestureRecognizer:doubleTapGestureRecognizer];
        
        [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];
        
        UISwipeGestureRecognizer *swipeGestureRecognizer=[[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
        [self addGestureRecognizer:swipeGestureRecognizer];
        
        UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
        [self addGestureRecognizer:longPressGestureRecognizer];
        
    }
    return(self);
}

-(void)drawRect:(CGRect)rect
{
    //black color background
    UIColor *fillColor = [UIColor blackColor];
    [fillColor set];
    UIRectFill(rect);
    
    //Dictionery with kvc
    NSDictionary * dictionaryForTextAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                                  [UIFont fontWithName:@"Helvetica" size:24],NSFontAttributeName,
                                                  [UIColor greenColor],NSForegroundColorAttributeName,
                                                  nil];
    
    CGSize textSize = [centralText sizeWithAttributes:dictionaryForTextAttributes];
    
    
    CGPoint point;
    point.x=(rect.size.width/2)-(textSize.width/2);
    point.y=(rect.size.height/2)-(textSize.height/2)+12;
    
    [centralText drawAtPoint:point withAttributes:dictionaryForTextAttributes];
}


-(BOOL)acceptsFirstResponder
{
    return(YES);
}

-(void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    centralText = @"touchesBegan'EventOccurred";
    [self setNeedsDisplay];//repainting
    
}

-(void)onSingletap:(UITapGestureRecognizer*)gr
{
    centralText = @"onSingleTap'EventOccurred";
    [self setNeedsDisplay];//repainting
}

-(void)onDoubleTap:(UITapGestureRecognizer*)gr
{
    centralText = @"onDoubleTap'EventOccurred";
    [self setNeedsDisplay];//repainting
}

-(void)onSwipe:(UISwipeGestureRecognizer*)gr
{
    [self release];
    exit(0);
}


-(void)onLongPress:(UILongPressGestureRecognizer*)gr
{
    centralText = @"onLongPress'EventOccurred";
    [self setNeedsDisplay];//repainting
}

-(void)dealloc
{
    [super dealloc];
}

@end
